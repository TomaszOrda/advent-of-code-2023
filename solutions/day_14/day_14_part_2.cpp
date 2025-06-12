#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_map>

#define ROUND_ROCK 'O'
#define CUBE_SHAPED_ROCK '#'
#define EMPTY_SPACE '.'

std::string grid_to_string(Grid<char> g){
    std::string result{};
    for (int x{}; x<g.get_width(); x++){
        for (int y{}; y<g.get_height(); y++){
            result += g.at(x,y).value();
        }
        result += '\n';
    }
    return result;
}

class ParabolicDish{
    Grid<char> rocks;
public:
    ParabolicDish(std::string_view rocks_map) : rocks{rocks_map}
    {}
    int beam_load(Coord beam_direction){
        int total_load = 0;
        for (int x{}; x<rocks.get_width(); x++){
            for (int y{}; y<rocks.get_height(); y++){
                if (rocks.at(x,y).value() == ROUND_ROCK){
                    if (beam_direction == Coord::West)
                        total_load += static_cast<int>(rocks.get_width()) - x;
                    else if (beam_direction == Coord::East)
                        total_load += x+1;
                    else if (beam_direction == Coord::North)
                        total_load += static_cast<int>(rocks.get_height()) - y;
                    else if (beam_direction == Coord::South)
                        total_load += y+1;
                }
            }
        }
        return total_load;
    }
    void tilt(Coord direction){
        int start_1, end_1;
        int start_2, end_2;
        if (direction == Coord::North){
            start_1 = 0;
            end_1 = rocks.get_width();
            start_2 = 0;
            end_2 = rocks.get_height();
        }else if (direction == Coord::South){
            start_1 = rocks.get_width()-1;
            end_1 = -1;
            start_2 = rocks.get_height()-1;
            end_2 = -1;
        }else if (direction == Coord::West){
            start_1 = 0;
            end_1 = rocks.get_height();
            start_2 = 0;
            end_2 = rocks.get_width();
        }else if (direction == Coord::East){
            start_1 = rocks.get_height()-1;
            end_1 = -1;
            start_2 = rocks.get_width()-1;
            end_2 = -1;
        } else{
            throw std::runtime_error("Unknown direction");
        }
        Coord direction_1_opposite, direction_2_opposite;
        for (int i_x{start_1}; i_x!= end_1; i_x+= (end_1-start_1)/abs(end_1-start_1)){
            Coord stopper = 
                direction == Coord::North ? Coord{i_x,-1} : 
                (direction == Coord::West ? Coord{-1, i_x} :
                (direction == Coord::South ? Coord{i_x, static_cast<int>(rocks.get_height())} : 
                Coord{static_cast<int>(rocks.get_width()), i_x} ));
            for (int i_y{start_2}; i_y != end_2; i_y+= (end_2-start_2)/abs(end_2-start_2)){
                int x = direction == Coord::North || direction == Coord::South ? i_x : i_y;
                int y = direction == Coord::North || direction == Coord::South ? i_y : i_x;
                switch (rocks.at(x,y).value())
                {
                case CUBE_SHAPED_ROCK:
                    stopper = Coord(x,y);
                    break;
                case ROUND_ROCK:
                    stopper = stopper + direction.rotate_left().rotate_left();
                    rocks.replace(x, y, EMPTY_SPACE);
                    rocks.replace(stopper, ROUND_ROCK);
                    break;
                case EMPTY_SPACE:
                    break;
                default:
                    throw std::runtime_error("Invalid map character" + std::string(1, rocks.at(i_x,y).value()));
                }
            }
        }
    }
    void spin(int spin_cycles){
        std::unordered_map<std::string, int> cache{};
        bool skip_found = false;
        std::string key = grid_to_string(rocks);
        while(spin_cycles>0){
            key = grid_to_string(rocks);
            if (!skip_found && cache.contains(key)){
                skip_found = true;
                int skip_length = spin_cycles - cache.at(key);
                spin_cycles = spin_cycles % skip_length;
            }
            cache.emplace(key, spin_cycles);
            tilt(Coord::North);
            tilt(Coord::West);
            tilt(Coord::South);
            tilt(Coord::East);
            spin_cycles--;
        }
    }
};

std::optional<std::string> solve(std::string_view input){
    ParabolicDish parabolic_dish(input);
    parabolic_dish.spin(1000000000);
    return std::to_string(parabolic_dish.beam_load(Coord::North));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(14, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
