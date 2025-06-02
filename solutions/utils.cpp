#include "utils.hpp"
#include <string_view>
#include <vector>
#include <charconv>

//The logic could be optimized by the use of iterators.
//This could save the overhead of holding a vector of string_views.
std::vector<std::string_view> split(std::string_view text, char delimiter){
    std::vector<std::string_view> result{};
    size_t line_start = 0;
    for (size_t i{0}; i<text.length(); i++){
        char c = text[i];
        if (c == delimiter){
            result.push_back(text.substr(line_start, i-line_start));
            line_start = i+1;
        }
    }
    if (line_start != text.length()){
        result.push_back(text.substr(line_start, text.length() - line_start));
    }
    return result;
}

std::vector<std::string_view> split_lines(std::string_view text){
    return split(text, '\n');
}

int svtoi(std::string_view s){
    int value{};
    std::from_chars(s.data(), s.data() + s.size(), value);
    return value;
}
