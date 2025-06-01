#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Subset{
public:
    std::unordered_map<std::string_view, int> cubes ={
        {"red", 0}, {"green", 0}, {"blue", 0}
    };
    Subset(std::string_view subset_record){
        for (const std::string_view& cube_record : split(subset_record, ',')){
            std::vector<std::string_view> cube_record_split {split(cube_record, ' ')};
            std::string cube_type {cube_record_split[2]};
            int number_of_cubes {svtoi(cube_record_split[1])};
            cubes[cube_type] = number_of_cubes;
        }
    }
    Subset(int red, int green, int blue){
        cubes ={
            {"red", red}, {"green", green}, {"blue", blue}
        };
    }
    bool is_possible(const std::unordered_map<std::string_view, int>& max_cubes) const{
        return max_cubes.at("red") >= cubes.at("red") && 
            max_cubes.at("green") >= cubes.at("green") &&
            max_cubes.at("blue") >= cubes.at("blue");
    }
    Subset operator|(const Subset& b) const{
        return Subset(
            std::max(cubes.at("red"), b.cubes.at("red")),
            std::max(cubes.at("green"), b.cubes.at("green")),
            std::max(cubes.at("blue"), b.cubes.at("blue"))
        );
    }
    int subset_power() const{
        return cubes.at("red") * cubes.at("green") * cubes.at("blue");
    }
};

class Game{
public:
    int id{};

    Game(std::string_view game_record){
        size_t header_pos = game_record.find(':');
        id = svtoi(game_record.substr(5, header_pos-5));
        for (const std::string_view& subset_record : split(game_record.substr(header_pos), ';')){
            subsets.push_back(Subset(subset_record));
        }
    }
    bool is_possible(const std::unordered_map<std::string_view, int> max_cubes) const{
        for (const Subset& subset : subsets){
            if (!subset.is_possible(max_cubes)){
                return false;
            }
        }
        return true;
    }
    Subset minimal_subset() const{
        Subset result = Subset(0,0,0);
        for (const Subset& subset : subsets){
            result = result | subset;
        }
        return result;
    }
private:
    std::vector<Subset> subsets{};
    
};

std::optional<std::string> solve(std::string_view input){
    int sum_of_minimal_game_powers {0};
    for (const std::string_view& line : split_lines(input)){
        Game game(line);
        sum_of_minimal_game_powers += game.minimal_subset().subset_power();
    }
    return std::to_string(sum_of_minimal_game_powers);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(2, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
