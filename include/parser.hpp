#pragma once

#include <string>
#include <vector>

#include "types.hpp"

std::vector<Tab> parse_tabs(const std::string &window_list_json);
