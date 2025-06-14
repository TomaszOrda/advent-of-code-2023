#include "utils.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <charconv>

#include<iostream>
//The logic could be optimized by the use of iterators.
//This could save the overhead of holding a vector of string_views.
std::vector<std::string_view> split(std::string_view text, std::string_view delimiter, bool only_nonempty){
    std::vector<std::string_view> result{};
    if (text.length() == 0){
        return result;
    }
    size_t line_start = 0;
    while (true){
        size_t line_end = text.find(delimiter, line_start);
        if (!only_nonempty || line_end - line_start > 0){
            result.push_back(text.substr(line_start, line_end - line_start));
        }
        if (line_end <= text.length()+ delimiter.length()){
            line_start = line_end + delimiter.length();
            //Make sure that line_start does not roll over. 
        }else{
            break;
        };
    };
    return result;
}
std::vector<std::string_view> split(std::string_view text, char delimiter, bool only_nonempty){
    std::string_view sv_delimiter(&delimiter, 1);
    return split(text, sv_delimiter, only_nonempty);
}

std::vector<std::string_view> split_lines(std::string_view text, bool only_nonempty){
    return split(text, '\n', only_nonempty);
}

int svtoi(std::string_view s){
    int value{};
    std::from_chars(s.data(), s.data() + s.size(), value);
    return value;
}
long long int svtoll(std::string_view s){
    long long int  value{};
    std::from_chars(s.data(), s.data() + s.size(), value);
    return value;
}
int is_digit(char c){
    return '0' <= c && c <= '9';
}
int digit_to_int(char c){
    return c - '0';
}



Coord Coord::operator+(const Coord& other) const{
    return Coord(first + other.first, second + other.second);
}
Coord Coord::operator-(const Coord& other) const{
    return Coord(first - other.first, second - other.second);
}
Coord Coord::operator-() const{
    return Coord(-first, -second);
}
bool Coord::operator==(const Coord& other) const{
    return first == other.first && second == other.second;
}
Coord Coord::rotate_left() const{
    return Coord(second, -first);
}
Coord Coord::rotate_right() const{
    return rotate_left().rotate_left().rotate_left();
}
int Coord::manhattan_distance() const{
    return abs(first) + abs(second);
}
Coord::operator std::pair<int, int>() const {
    return {first, second};
}
constexpr Coord Coord::North = Coord(0, -1);
constexpr Coord Coord::South = Coord(0, 1);
constexpr Coord Coord::East = Coord(1, 0);
constexpr Coord Coord::West = Coord(-1, 0);