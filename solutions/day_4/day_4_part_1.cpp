#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <set>
#include <algorithm>
#include <cmath>

class Card{
public:
    Card(std::string_view record){
        auto header_body = split(record, ':');
        id = svtoi(split(header_body[0], ' ')[1]);
        auto numbers_list = split(header_body[1], '|');
        for (const std::string_view& number : split(numbers_list[0], ' ')){
            if (!number.empty()){
                numbers.insert(svtoi(number));
            }
        }
        for (const std::string_view& number : split(numbers_list[1], ' ')){
            if (!number.empty()){
                winning_numbers.insert(svtoi(number));
            }
        }
    }
    int value(){
        std::set<int> matches{};
        std::set_intersection(
            numbers.begin(), numbers.end(), 
            winning_numbers.begin(), winning_numbers.end(), 
            std::inserter(matches, matches.begin()));
        if (matches.empty()) {
            return 0;
        }else{
            return int(1) << (matches.size() - 1);
        }
    }
private:
    int id{};
    std::set<int> numbers{};
    std::set<int> winning_numbers{};
};

std::optional<std::string> solve(std::string_view input){
    int total_cards_value{};
    for (const std::string_view& card_record : split_lines(input)){
        total_cards_value+= Card(card_record).value();
    }
    return std::to_string(total_cards_value);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(4, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
