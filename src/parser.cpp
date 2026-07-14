#include <nlohmann/json.hpp>
#include <vector>

#include "parser.hpp"

using json = nlohmann::json;

namespace kitty_tabs {
    std::vector<Tab> parse_tabs(const std::string& window_list_json)
    {
        std::vector<Tab> result;

        auto window_list = json::parse(window_list_json);

        if (window_list.is_array())
        {
            for (const auto& window : window_list)
            {
                auto tabs = window["tabs"];

                if (tabs.is_array())
                {
                    for (const auto& tab : tabs)
                    {
                        Tab tb{tab["id"], tab["title"]};
                        result.push_back(tb);
                    }
                }
            }
        }

        return result;
    }
} // namespace kitty_tabs
