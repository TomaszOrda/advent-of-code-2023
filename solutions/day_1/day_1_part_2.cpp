#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>

int starts_with_digit(std::string_view s, bool reverse=false){
    auto pattern_matcher = [reverse](std::string_view s, std::string_view pattern){
        return reverse ? s.ends_with(pattern) : s.starts_with(pattern);
    };
    char first_character = reverse ? s.back() : s[0];
    if ('0' <= first_character && first_character <= '9'){
        return first_character - '0';
    }else{
        std::string_view digits[10] 
            {"zero", "one", "two", 
            "three", "four", "five", 
            "six", "seven", "eight", "nine"};
        for (size_t i{0}; i < 10; i++){
            std::string_view pattern = digits[i];
            if (s.length() >= pattern.length() && pattern_matcher(s, pattern)){
                return i;
            }
        }
    }
    return -1;
}

std::optional<std::string> solve(std::string_view input){
    int sum_of_calibraction_values = 0;
    for (const std::string_view& line : split_lines(input)){
        int first_digit {-1};
        int last_digit {-1};

        for (size_t i=1; first_digit==-1 or last_digit==-1; i++){
            size_t i_back { line.length()-i };
            if (first_digit==-1){
                first_digit = starts_with_digit(line.substr(0,i), true);
            }
            if (last_digit==-1){
                last_digit = starts_with_digit(line.substr(i_back,i));
            }
        }
        int calibration_value = 10*first_digit + last_digit;
        sum_of_calibraction_values += calibration_value;
    }
    return std::to_string(sum_of_calibraction_values);
}

int main(int argc, char *argv[]) {
    Solution solution = Solution(1, 2, solve);
    if (argc > 1){
        return solution.run("day_1_test_2.txt");
    }else{
        return solution.run();
    }
}
