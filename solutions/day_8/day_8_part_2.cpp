#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <ranges>

#define START_SUFFIX 'A'
#define END_SUFFIX 'Z'

//I am assuming several additional facts about the input:
//Each ghost's path cycles at the endpoint point
//During each cycle a ghost will visit only one endpoint
//The path from start to the endpoint is the same as the aforementioned cycle length

//It is necessary for keeping my code nice and short
//From my experience (2024 and 2022 AoC) some puzzles do require looking at/testing the input
//to find a (short) solution.

template<typename It>
constexpr typename std::iterator_traits<It>::value_type lcm(It begin, It end){
    using T = typename std::iterator_traits<It>::value_type;
    T result = 1;
    for (It it = begin; it != end; ++it) {
        result = std::lcm(result, *it);
    }
    return result;
};

class DirectionFeed{
    std::string_view directions{};
    size_t current_id{0};
public:
    DirectionFeed(){};
    DirectionFeed(std::string_view directions_record) : directions{directions_record}
    {   
    }
    char get_direction(){
        size_t previous_id = current_id;
        current_id = (previous_id+1) % directions.length();
        return directions[previous_id];
    }
    char peek_direction() const{
        return directions[current_id];
    }
    void reset(){
        current_id = 0;
    }
};

class Node{
private:
    std::string left_destination{};
    std::string right_destination{};
public:
    std::string name{};
    Node(std::string_view node_record){
        std::string s(node_record);
        s.erase(std::remove_if(s.begin(), s.end(), [](char c){return c=='=' || c==',' || c=='(' || c==')';}), s.end());
        auto parts = split(s, ' ', true);
        name = parts[0];
        left_destination = parts[1];
        right_destination = parts[2];
    }
    std::string_view get_destination(char direction){
        if (direction == 'L') return left_destination;
        if (direction == 'R') return right_destination;
        throw std::runtime_error("Invalid direction: " + std::string(1, direction));
    }
};

class Network{
    std::unordered_set<std::string> ghosts{};
    DirectionFeed direction_feed{};
    std::unordered_map<std::string, Node> nodes{};

public:
    Network(std::string_view pack_of_maps) {
        auto lines = split_lines(pack_of_maps);
        direction_feed = DirectionFeed(lines[0]);
        for (const auto& node : lines | std::views::drop(2) | std::views::transform([](auto x){return Node(x);})) {
            nodes.emplace(node.name, node);
            if (node.name.back() == START_SUFFIX){
                ghosts.emplace(node.name);
            }
        }
    }
    long long int distance_to_end(){
        std::unordered_set<long long int> distances{};
        for (const auto& ghost : ghosts)
            distances.emplace(distance_to_end(ghost));
        
        return lcm(distances.begin(), distances.end());
    }
private:
    int distance_to_end(std::string_view start_node){
        direction_feed.reset();
        std::string_view current = start_node;
        int distance{};
        while(!current.ends_with(END_SUFFIX)){
            current = take_step(current);
            distance++;
        }
        return distance;
    }
    std::string_view take_step(std::string_view current){
        std::string current_s{current};
        return nodes.at(current_s).get_destination(direction_feed.get_direction());
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(Network(input).distance_to_end());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(8, 2, solve);
    if (argc > 1){
        return solution.run("day_8_test_3.txt");
    }else{
        return solution.run();
    }
}
