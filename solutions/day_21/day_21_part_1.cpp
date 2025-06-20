#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_set>

#define ROCK '#'
#define START 'S'
#define PLOT '.'

#define STEPS_SMALL_GARDEN 6
#define STEPS_BIG_GARDEN 64

#define DIRECTIONS {Coord::North, Coord::East, Coord::South, Coord::West}

class Garden{
    Grid<char> plots;
    Coord start{};
public:
    Garden(std::string_view text) : plots{text}
    {
        bool found = false;
        for(int x{}; !found && x<plots.get_width();x++)
            for(int y{}; !found && y<plots.get_height();y++)
                if (plots.at(x,y).value() == START){
                    start = Coord(x,y);
                    plots.replace(start, PLOT);
                    found = true;
                }
    }
    int plots_reached(int steps){
        std::unordered_set<Coord> reached {start};
        for(int step{};step<steps;step++){
            std::unordered_set<Coord> next{};
            for (const auto& coord : reached){
                for (auto new_coord : surrounding_plots(coord)){
                    next.emplace(new_coord);
                }
            }
            reached = next;
        }
        return static_cast<int>(reached.size());
    }

    std::unordered_set<Coord> surrounding_plots(Coord pos){
        std::unordered_set<Coord> result;
        for (const auto& dir : DIRECTIONS){
            Coord new_pos = pos + dir;
            if (plots.at(new_pos) && plots.at(new_pos).value()==PLOT)
                result.emplace(new_pos);
        }
        return result;
    }

    std::string garden_type(){
        if (plots.get_width()>16)
            return "big";
        return "small";
    }
};

std::optional<std::string> solve(std::string_view input){
    Garden garden(input);
    int steps = garden.garden_type() == "big" ? STEPS_BIG_GARDEN : STEPS_SMALL_GARDEN;
    return std::to_string(garden.plots_reached(steps));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(21, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
