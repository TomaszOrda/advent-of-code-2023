#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <algorithm>
#include <tuple>

#define HAND_SIZE 5
#define FIVE_OF_A_KIND_STRENGTH 7
#define FOUR_OF_A_KIND_STRENGTH 6
#define FULL_HOUSE_STRENGTH 5
#define THREE_OF_A_KIND_STRENGTH 4
#define TWO_PAIR_STRENGTH 3
#define PAIR_STRENGTH 2
#define HIGH_CARD_STRENGTH 1

#define CARD_VALUES {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'}
#define CARD_VALUES_SIZE 13

class Card{
    int strength{};
public:
    Card(){}
    Card(char c){
        constexpr std::array<char, CARD_VALUES_SIZE> card_values_ordered CARD_VALUES;
        strength = static_cast<int>(std::distance(
                card_values_ordered.begin(), 
                std::find(card_values_ordered.begin(), card_values_ordered.end(), c)
            ));
    }
    int get_strength() const{
        return strength;
    }
    bool operator<(const Card& b) const{
        return get_strength() < b.get_strength();
    }
    bool operator==(const Card& b) const{
        return get_strength() == b.get_strength();
    }
};

class Hand{
    std::array<Card, HAND_SIZE> cards{};
public:
    Hand(){}
    Hand(std::string_view hand_record){
        for (size_t i{0}; i<HAND_SIZE;i++){
            cards[i] = Card(hand_record[i]);
        }
    }
    int get_hand_strength() const{
        std::array<int, CARD_VALUES_SIZE> card_counts{};
        constexpr std::array<char, CARD_VALUES_SIZE> card_values_ordered CARD_VALUES;
        for (size_t i{0}; i<CARD_VALUES_SIZE;i++){
            if (card_values_ordered[i]!='J'){
                card_counts[i] = static_cast<int>(std::count(cards.begin(), cards.end(), card_values_ordered[i]));
            }else{
                card_counts[i] = 0;
            }
        }
        int jokers = static_cast<int>(std::count(cards.begin(), cards.end(), 'J'));
        std::sort(card_counts.begin(), card_counts.end(), std::greater<>());
        switch (card_counts[0]+jokers)
        {
        case 5:
            return FIVE_OF_A_KIND_STRENGTH;
        case 4:
            return FOUR_OF_A_KIND_STRENGTH;
        case 3:
            if (card_counts[1] == 2){
                return FULL_HOUSE_STRENGTH;
            }else{
                return THREE_OF_A_KIND_STRENGTH;
            }
        case 2:
            if (card_counts[1] == 2){
                return TWO_PAIR_STRENGTH;
            }else{
                return PAIR_STRENGTH;
            }
        default:
            return HIGH_CARD_STRENGTH;
        }
    }
    Card get_card(size_t pos) const{
        return cards[pos];
    }
    bool operator<(const Hand& b) const{
        if (get_hand_strength() != b.get_hand_strength()){
            return get_hand_strength() < b.get_hand_strength();
        }else{
            for (size_t i{0}; i<HAND_SIZE; i++){
                if (get_card(i) != b.get_card(i)){
                    return get_card(i) < b.get_card(i);
                }
            }
        }
        return false;
    }
};
class HandAndBid : public Hand{
    int bid{};
public:
    HandAndBid(std::string_view hand_and_bid_record) 
        // Not sure which version is better
        // : Hand(split(hand_and_bid_record, ' ')[0])
        // , bid {svtoi(split(hand_and_bid_record, ' ')[1])} 
    {
        auto parts = split(hand_and_bid_record, ' ');
        static_cast<Hand&>(*this) = Hand(parts[0]);
        bid = svtoi(parts[1]);
    }
    int get_bid(){
        return bid;
    }
};

std::optional<std::string> solve(std::string_view input){
    auto lines = split_lines(input);
    std::vector<HandAndBid> set_of_hands;
    for (const auto& line : lines){
        set_of_hands.push_back(HandAndBid(line));
    }

    std::sort(set_of_hands.begin(), set_of_hands.end());

    int total_winnings{};
    for (size_t i{0}; i<set_of_hands.size(); i++){
        int rank = static_cast<int>(i + 1);
        total_winnings += rank * set_of_hands[i].get_bid();
    }
    return std::to_string(total_winnings);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(7, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
