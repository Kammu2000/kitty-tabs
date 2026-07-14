#pragma once

#include <optional>
#include <string>
#include <vector>

std::string run_command_capture_output(const std::vector<std::string>& args,
                                       std::optional<std::string> input = std::nullopt);
