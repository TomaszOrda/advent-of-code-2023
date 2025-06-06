#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>

#define START "AAA"
#define END "ZZZ"

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
    std::string current{START};
    DirectionFeed direction_feed{};
    std::unordered_map<std::string, Node> nodes{};

public:
    Network(std::string_view pack_of_maps) {
        auto lines = split_lines(pack_of_maps);
        direction_feed = DirectionFeed(lines[0]);
        lines.erase(lines.begin());
        lines.erase(lines.begin());
        for (const auto& line : lines) {
            Node node(line);
            nodes.emplace(node.name, node);
        }
    }
    int distance_to_end(){
        int distance{};
        while(current != END){
            take_step();
            distance++;
        }
        return distance;
    }
private:
    void take_step(){
        current = nodes.at(current).get_destination(direction_feed.get_direction());
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(Network(input).distance_to_end());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(8, 1, solve);
    if (argc > 1){
        solution.test_run();
        std::cout<<'\n';
        return solution.run("day_8_test_2.txt");
    }else{
        return solution.run();
    }
}
