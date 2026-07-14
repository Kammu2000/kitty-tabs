#pragma once

#include <string>
#include <vector>

#include "types.hpp"

namespace kitty_tabs {
    std::vector<Tab> parse_tabs(const std::string& window_list_json);
}
