#include <print>
#include <unistd.h>

#include "pipe.hpp"
#include "process.hpp"

namespace {

    std::vector<char*> get_c_args(const std::vector<std::string>& args)
    {
        std::vector<char*> argv;
        argv.reserve(args.size() + 1);

        for (const auto& arg : args)
        {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        argv.push_back(nullptr);
        return argv;
    }

    std::string read_all_from_pipe(Pipe& pipe, pid_t pid)
    {
        std::string result;
        constexpr size_t BUFFER_SIZE = 4096;
        std::array<char, BUFFER_SIZE> buffer;

        while (true)
        {
            const ssize_t bytes = read(pipe.read_fd(), buffer.data(), buffer.size());

            if (bytes == -1)
            {
                throw std::system_error(errno, std::system_category(), "read");
            }

            if (bytes == 0)
            {
                break;
            }

            result.append(buffer.data(), static_cast<size_t>(bytes));
        }

        pipe.close_read();
        waitpid(pid, nullptr, 0);
        return result;
    }

} // namespace

std::string run_command_capture_output(const std::vector<std::string>& args,
                                       std::optional<std::string> input)
{
    std::vector<char*> argv = get_c_args(args);
    std::optional<Pipe> input_pipe;

    // NOTE : input is present if parent process needs to write something in stdin so that child can read that
    if (input)
    {
        input_pipe.emplace();
    }

    Pipe output_pipe;
    const pid_t pid = fork();

    if (pid == -1)
    {
        throw std::system_error(errno, std::system_category(), "fork");
    }

    if (pid == 0)
    {
        if (input_pipe)
        {
            if (dup2(input_pipe->read_fd(), STDIN_FILENO) == -1)
            {
                _exit(EXIT_FAILURE);
            }
            input_pipe->close_write();
        }

        if (dup2(output_pipe.write_fd(), STDOUT_FILENO) == -1)
        {
            _exit(EXIT_FAILURE);
        }

        output_pipe.close_read();
        execvp(argv[0], argv.data());

        std::print(stderr, "Failed to execute {}: {}\n", argv[0], strerror(errno));
        _exit(EXIT_FAILURE);
    }

    if (input_pipe)
    {
        input_pipe->close_read();
    }
    output_pipe.close_write();

    if (input && !input->empty())
    {
        const ssize_t written = write(input_pipe->write_fd(), input->data(), input->size());
        if (written == -1)
        {
            throw std::system_error(errno, std::system_category(), "write");
        }
    }

    if (input_pipe)
    {
        input_pipe->close_write();
    }

    return read_all_from_pipe(output_pipe, pid);
}
