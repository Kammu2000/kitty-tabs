#include <cstddef>
#include <format>

#include "kitty.hpp"
#include "process.hpp"

namespace kitty_tabs {
    namespace {
        std::string build_fzf_input(const std::vector<Tab>& tabs)
        {
            std::string out;

            for (const auto& tab : tabs)
            {
                out = out + std::format("{}:{}\n", tab.id, tab.title);
            }

            return out;
        }

    } // namespace

    std::string kitty_list_tabs()
    {
        return run_command_capture_output({"kitty", "@", "ls"});
    }

    int pick_tab(const std::vector<Tab>& tabs)
    {
        std::string input = build_fzf_input(tabs);

        if (input.empty())
        {
            return -1;
        }

        std::vector<std::string> args = {
            "/opt/homebrew/bin/fzf", "--height=50%", "--reverse", "--border", "--prompt=Tabs> ",
        };

        std::string output = run_command_capture_output(args, std::move(input));
        std::size_t colon_idx = output.find(':');

        if (colon_idx == std::string::npos)
        {
            return -1;
        }

        std::string id = output.substr(0, colon_idx + 1);
        return std::stoi(id);
    }

    void focus_tab(int tab_id)
    {
        (void)run_command_capture_output(
            {"kitty", "@", "focus-tab", "--match", std::format("id:{}", tab_id)});
    }
} // namespace kitty_tabs
