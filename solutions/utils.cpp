#include "utils.hpp"
#include <string_view>
#include <vector>

std::vector<std::string_view> split_lines(std::string_view text){
    std::vector<std::string_view> result{};
    size_t line_start = 0;
    for (size_t i{0}; i<text.length(); i++){
        char c = text[i];
        if (c == '\n'){
            result.push_back(text.substr(line_start, i-line_start));
            line_start = i+1;
        }
    }
    if (line_start != text.length()){
        result.push_back(text.substr(line_start, text.length() - line_start));
    }
    return result;
}