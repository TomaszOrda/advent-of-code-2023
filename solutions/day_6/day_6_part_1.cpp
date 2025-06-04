#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <numeric>
#include <cmath>

class Race{
public:
    Race(int time, int distance) : m_time{time}, m_distance{distance} {};
    int ways_to_win(){
        double delta = std::sqrt(std::pow(m_time, 2) - 4* m_distance);
        if (delta<0){
            return 0;
        }
        int min_winning_hold = static_cast<int>(std::floor((m_time - delta)/2)) + 1;
        int max_winning_hold = static_cast<int>(std::ceil((m_time + delta)/2)) - 1;
        return max_winning_hold - min_winning_hold + 1;
    }

private:
    int m_time;
    int m_distance;
};

std::optional<std::string> solve(std::string_view input){
    auto lines = split_lines(input);
    auto time_data = split(lines[0], ' ', true);
    auto distance_data = split(lines[1], ' ', true);
    std::vector<Race> races{};
    for (size_t i{1}; i<time_data.size(); i++){
        races.push_back(Race(svtoi(time_data[i]), svtoi(distance_data[i])));
    }
    int total_ways_to_win{std::accumulate(
            races.begin(), 
            races.end(), 
            1, 
            [](int a, Race b){return a * b.ways_to_win();}
        )};
    return std::to_string(total_ways_to_win);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(6, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
