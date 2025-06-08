#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <deque>

#define SOUTH Coord(0,1)
#define NORTH Coord(0,-1)
#define WEST Coord(-1,0)
#define EAST Coord(1,0)
#define DIRECTIONS {SOUTH, NORTH, WEST, EAST}

class Coord{
public:
    int first;
    int second;
    Coord(): first{0}, second{0} {};
    Coord(std::pair<int, int> pair) : first{pair.first}, second{pair.second} {};
    Coord(int x, int y) : first{x}, second{y}{}
    Coord operator+(const Coord& other) const{
        return Coord(first + other.first, second + other.second);
    }
    bool operator==(const Coord& other) const{
        return first == other.first && second == other.second;
    }
};

class PipeSystem{
    Grid<char> pipe_grid;
    Coord start{};
public:
    PipeSystem(std::string_view pipe_system_map) : pipe_grid{pipe_system_map} {
        start = pipe_grid.find('S');
    }
    int pipe_grid_diameter(){
        int diameter = 0;
        std::deque<Coord> stack(1, start);
        Grid<int> distance_grid(pipe_grid.get_width(), pipe_grid.get_height(), INT_MAX);
        distance_grid.replace(start.first, start.second, 0);
        while(!stack.empty()){
            Coord pos = stack.front();
            int distance = distance_grid.at(pos.first, pos.second).value();
            diameter = std::max(diameter, distance);
            auto next_positions = connects_to(pos).value();
            for (const auto& next_pos : {next_positions.first, next_positions.second}){
                if (distance + 1 < distance_grid.at(next_pos.first, next_pos.second).value()){
                    distance_grid.replace(next_pos.first, next_pos.second, distance + 1);
                    stack.push_back(next_pos);
                }
            }
            stack.pop_front();
        }
        return diameter;
    }
private:
    std::optional<std::pair<Coord, Coord>> connects_to(Coord pos){
        switch (pipe_grid.at(pos.first, pos.second).value())
        {
        case '|':
            return {{pos + SOUTH, pos + NORTH}};
        case '-':
            return {{pos + WEST, pos + EAST}};
        case 'L':
            return {{pos + NORTH, pos + EAST}};
        case 'J':
            return {{pos + NORTH, pos + WEST}};
        case '7':
            return {{pos + WEST, pos + SOUTH}};
        case 'F':
            return {{pos + EAST, pos + SOUTH}};
        case 'S':
            {
                std::vector<Coord> result{};
                for (const auto& dir : DIRECTIONS){
                    if (connects_to(pos + dir) && 
                        (connects_to(pos + dir).value().first == pos || connects_to(pos + dir).value().second == pos)){
                        result.push_back(dir + pos);
                    }
                }
                return {{result[0], result[1]}};
            }
        default:
            return std::nullopt;
        }
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(PipeSystem(input).pipe_grid_diameter());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(10, 1, solve);
    if (argc > 1){
        solution.test_run();
        std::cout<<'\n';
        return solution.run("day_10_test_2.txt");
    }else{
        return solution.run();
    }
}
