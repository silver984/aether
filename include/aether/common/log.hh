#pragma once
#include <string_view>
#include <source_location>

namespace ae::log {

void trace(std::string_view msg, std::source_location const& loc = std::source_location::current());
void debug(std::string_view msg, std::source_location const& loc = std::source_location::current());
void info(std::string_view msg, std::source_location const& loc = std::source_location::current());
void warn(std::string_view msg, std::source_location const& loc = std::source_location::current());
void error(std::string_view msg, std::source_location const& loc = std::source_location::current());

}