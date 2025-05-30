#include "../runner_oo.hpp"
#include "../utils.cpp"
#include <string>
#include <optional>
#include <string_view>
#include <sstream>

int is_digit(char c){
    return '0' <= c && c <= '9';
}
int digit_to_int(char c){
    return c - '0';
}

std::optional<std::string> solve(std::string_view input){
    int sum_of_calibraction_values = 0;
    for (std::string_view line : split_lines(input)){
        char first_digit {};
        char last_digit {};

        for (size_t i=0; !first_digit or !last_digit; i++){
            size_t i_back = line.length()-i;
            if (!first_digit && is_digit(line[i])){
                first_digit = line[i];
            }
            if (!last_digit && is_digit(line[i_back])){
                last_digit = line[i_back];
            }
        }
        int calibration_value = 10*digit_to_int(first_digit) + digit_to_int(last_digit);
        sum_of_calibraction_values += calibration_value;
    }
    return std::to_string(sum_of_calibraction_values);
}

int main(int argc, char *argv[]) {
    Solution solution = Solution(1, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
