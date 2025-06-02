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
        id = static_cast<size_t>(svtoi(split(header_body[0], ' ').back()));
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
    size_t matchings() const{
        std::set<int> matches{};
        std::set_intersection(
            numbers.begin(), numbers.end(), 
            winning_numbers.begin(), winning_numbers.end(), 
            std::inserter(matches, matches.begin()));
        return matches.size();
    }
    int get_cards_won(std::vector<Card>& cards){
        if (!cards_won_calculated){
            size_t card_pos = id -1;
            for(size_t i{1}; i<=matchings() && i+card_pos<cards.size(); i++){
                cards_won += 1 + cards[card_pos+i].get_cards_won(cards);
            }
            cards_won_calculated=true;
        }
        return cards_won;
    }
private:
    size_t id{};
    std::set<int> numbers{};
    std::set<int> winning_numbers{};
    int cards_won{};
    bool cards_won_calculated{false};
};

std::optional<std::string> solve(std::string_view input){
    std::vector<Card> cards{};
    for (const std::string_view& card_record : split_lines(input)){
        cards.push_back(Card(card_record));
    }
    int total_cards_held{};
    //Reverse the order so that the recursion is limited
    for (size_t i{cards.size()};i-->0;){
        Card& card = cards[i];
        total_cards_held += 1 + card.get_cards_won(cards);
    }
    return std::to_string(total_cards_held);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(4, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
