#include <aether/string.hh>
#include <algorithm>

namespace aether::util {

bool string_matches_any(std::string_view str, std::initializer_list<std::string_view> list) {
	return std::find(list.begin(), list.end(), str) != list.end();
}

} // namespace aether::util