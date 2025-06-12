#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>

#define ROUND_ROCK 'O'
#define CUBE_SHAPED_ROCK '#'
#define EMPTY_SPACE '.'


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
    void tilt_north(){
        for (int x{0}; x< rocks.get_width(); x++){
            Coord stopper{x,-1};
            for (int y{0}; y < rocks.get_height(); y++){
                switch (rocks.at(x,y).value())
                {
                case CUBE_SHAPED_ROCK:
                    stopper = Coord(x,y);
                    break;
                case ROUND_ROCK:
                    stopper.second++;
                    rocks.replace(x, y, EMPTY_SPACE);
                    rocks.replace(stopper, ROUND_ROCK);
                    break;
                case EMPTY_SPACE:
                    break;
                default:
                    throw std::runtime_error("Invalid map character" + std::string(1, rocks.at(x,y).value()));
                }
            }
        }
    }
};

std::optional<std::string> solve(std::string_view input){
    ParabolicDish parabolic_dish(input);
    parabolic_dish.tilt_north();
    return std::to_string(parabolic_dish.beam_load(Coord::North));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(14, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
