#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <ranges>
#include <deque>

#define DIRECTIONS {Coord::North, Coord::East, Coord::South, Coord::West}
#define EMPTY_TILE '.'
#define INSIDE_TILE 'I'
#define WALL_TILE '#'

class DigCommand{
public:
    Coord dir{};
    int length{};
    std::string color{};
    DigCommand(std::string_view text_command){
        auto parts = split(text_command, ' ');
        length = svtoi(parts[1]);
        color = std::string(parts[2].substr(2, parts[2].length()-3));
        switch (parts[0][0])
        {
        case 'U':
            dir = Coord::North;
            break;
        case 'R':
            dir = Coord::East;
            break;
        case 'D':
            dir = Coord::South;
            break;
        case 'L':
            dir = Coord::West;
            break;
        default:
            throw std::runtime_error("Unknown direction");
        }
    }
};

class Trench{
    std::vector<Coord> trench_cells{{ }};
    std::vector<DigCommand> commands{};
    Coord current_pos{0,0};
    Coord current_dir{0,0};
    int right_turns{};
    int min_x{}, max_x{}, min_y{}, max_y{};
public:
    Trench() {}
    void load_command(DigCommand command){
        if (current_dir.rotate_right() == command.dir){
            right_turns++;
        }else if (current_dir.rotate_left() == command.dir){
            right_turns--;
        }
        current_dir = command.dir;
        for(int step{}; step<command.length; step++){
            current_pos = current_pos + current_dir;
            trench_cells.push_back(current_pos);
        }
        commands.push_back(command);
    }
    void calculate_trench_bounds(){
        for (const Coord& c : trench_cells){
            min_x = std::min(min_x, c.first);
            max_x = std::max(max_x, c.first);
            min_y = std::min(min_y, c.second);
            max_y = std::max(max_y, c.second);
        }

    }
    int trench_volume(){
        if (max_x==0)
            calculate_trench_bounds();
        int clockwise = right_turns > 0 ? 1 : -1;
        double trench_volume = 0;
        Coord pos{0,0};
        Coord dir = commands.back().dir;
        for (const DigCommand& command : commands){
            //The area below does not include roughly half of the edge.
            double half_edge = 0;
            half_edge += static_cast<double>(command.length)/2;
            //Inside and outside corners must be counted sparately
            if (command.dir == dir.rotate_left())
                half_edge-=0.25;
            else if (command.dir == dir.rotate_right())
                half_edge+=0.25;
            
            trench_volume += half_edge;
            dir = command.dir;
            if (dir != Coord::East && dir != Coord::West){
                //Each area is counted twice (once from the left and once from the right). 
                //Outside area is counted once with plus and once with minus. 
                trench_volume += (0.5)*(dir == Coord::North ? -1 : 1) * clockwise * (command.length) * ((pos.first - min_x) - (max_x - pos.first));
            }
            pos = pos + Coord{dir.first*command.length, dir.second*command.length};
        }
        return  static_cast<int>(trench_volume);
    }
};

std::optional<std::string> solve(std::string_view input){
    Trench trench{};
    for (const DigCommand& command : split_lines(input) | std::views::transform([](auto x){return DigCommand(x);})){
        trench.load_command(command);
    }
    trench.calculate_trench_bounds();
    return std::to_string(trench.trench_volume());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(18, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
