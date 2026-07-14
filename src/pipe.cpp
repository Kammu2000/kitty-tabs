#include <cerrno>
#include <system_error>
#include <unistd.h>

#include "pipe.hpp"

Pipe::Pipe()
{
    if (::pipe(fd.data()) == -1)
    {
        throw std::system_error(errno, std::system_category(), "Pipe() creation failed");
    }
}

Pipe::~Pipe()
{
    close_fd();
}

Pipe::Pipe(Pipe&& other) noexcept : fd(other.fd)
{
    other.fd = {-1, -1};
}

Pipe& Pipe::operator=(Pipe&& other) noexcept
{
    if (this != &other)
    {
        close_fd();

        fd = std::move(other.fd);
        other.fd = {-1, -1};
    }

    return *this;
};

int Pipe::read_fd() const
{
    return fd[0];
};
int Pipe::write_fd() const
{
    return fd[1];
};

void Pipe::close_read()
{
    if (fd[0] != -1)
    {
        close(fd[0]);
        fd[0] = -1;
    }
};

void Pipe::close_write()
{
    if (fd[1] != -1)
    {
        close(fd[1]);
        fd[1] = -1;
    }
};
