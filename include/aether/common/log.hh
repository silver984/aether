#pragma once
#include <string_view>
#include <source_location>

namespace ae::log {

void info(std::string_view msg, std::source_location const& loc = std::source_location::current());

}