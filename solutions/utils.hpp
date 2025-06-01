#include <string_view>
#include <vector>

std::vector<std::string_view> split_lines(std::string_view text);
std::vector<std::string_view> split(std::string_view text, char delimiter);
int svtoi(std::string_view s);