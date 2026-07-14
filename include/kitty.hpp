#pragma once

#include <string>
#include <vector>

#include "types.hpp"

namespace kitty_tabs {
    std::string kitty_list_tabs();
    int pick_tab(const std::vector<Tab>& tabs);
    void focus_tab(int tab_id);
} // namespace kitty_tabs
