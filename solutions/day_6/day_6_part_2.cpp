#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <numeric>
#include <cmath>
#include <algorithm>

std::string extract_digits(std::string_view text){
    std::string s(text);
    s.erase(std::remove_if(s.begin(), s.end(), [](char c){return c<'0' || '9'<c;}), s.end());
    return s;
}

class Race{
public:
    Race(long long int time, long long int distance) : m_time{time}, m_distance{distance} {};
    long long int ways_to_win(){
        long double delta = std::sqrt(std::pow(m_time, 2) - static_cast<long double>(4* m_distance));
        if (delta<0){
            return 0;
        }
        long long int min_winning_hold = static_cast<long long int>(std::floorl((static_cast<long double>(m_time) - delta)/2)) + 1;
        long long int max_winning_hold = static_cast<long long int>(std::ceill((static_cast<long double>(m_time) + delta)/2)) - 1;
        return max_winning_hold - min_winning_hold + 1;
    }

private:
    long long int m_time;
    long long int m_distance;
};

std::optional<std::string> solve(std::string_view input){
    auto lines = split_lines(input);
    auto time_data{lines[0]};
    auto distance_data{lines[1]};
    Race race = Race(
            svtoll(extract_digits(time_data)),
            svtoll(extract_digits(distance_data))
        );
    return std::to_string(race.ways_to_win());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(6, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
