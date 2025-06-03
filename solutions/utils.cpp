#include "utils.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <charconv>

#include<iostream>
//The logic could be optimized by the use of iterators.
//This could save the overhead of holding a vector of string_views.
std::vector<std::string_view> split(std::string_view text, std::string_view delimiter){
    std::vector<std::string_view> result{};
    if (text.length() == 0){
        return result;
    }
    size_t line_start = 0;
    while (true){
        size_t line_end = text.find(delimiter, line_start);
        result.push_back(text.substr(line_start, line_end - line_start));
        if (line_end <= text.length()+ delimiter.length()){
            line_start = line_end + delimiter.length();
            //Make sure that line_start does not roll over. 
        }else{
            break;
        };
    };
    return result;
}
std::vector<std::string_view> split(std::string_view text, char delimiter){
    std::string_view sv_delimiter(&delimiter, 1);
    return split(text, sv_delimiter);
}

std::vector<std::string_view> split_lines(std::string_view text){
    return split(text, '\n');
}

int svtoi(std::string_view s){
    int value{};
    std::from_chars(s.data(), s.data() + s.size(), value);
    return value;
}
