#pragma once

#include <string>
#include <vector>

#include "types.hpp"

std::string kitty_list_tabs();
int pick_tab(const std::vector<Tab> &tabs);
void focus_tab(int tab_id);
