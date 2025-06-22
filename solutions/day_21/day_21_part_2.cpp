#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_set>
#include <unordered_map>

#define ROCK '#'
#define START 'S'
#define PLOT '.'


#define STEPS_SMALL_GARDEN 26501
#define STEPS_BIG_GARDEN 26501365

#define TAIL_GARDENS 3

#define DIRECTIONS {Coord::North, Coord::East, Coord::South, Coord::West}
#define CORNERS {Coord(-1,-1), Coord(-1,1), Coord(1,-1), Coord(1,1)}

template <>
struct std::hash<std::pair<Coord, Coord>> {
    std::size_t operator()(const std::pair<Coord, Coord>& p) const {
        return std::hash<Coord>()(p.first) ^ (std::hash<Coord>()(p.second) << 1);
    }
};

class Garden{
    Grid<char> plots;
    Coord start{};
    std::unordered_map<std::pair<Coord, Coord>, long long int> distance_from_to_cache{};
    std::unordered_map<Coord, std::vector<long long int>> single_garden_plots_visited_cache{};
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
    

    long long int plots_visited(long long int steps){
        //assume square garden
        long long int total = 0;

        long long int furthest_garden_visited = (steps - plots.get_width()/2 - 1) / static_cast<long long int>(plots.get_width()) + static_cast<long long int>(2); //intial garden is 1
        long long int furthest_full_garden = furthest_garden_visited - TAIL_GARDENS;
        long long int furthest_even_garden = 2*((furthest_full_garden-1)/2);
        long long int furthest_odd_garden = 2*((furthest_full_garden)/2) - 1;
        long long int full_gardens_even_parity = 4 * (furthest_even_garden/2)*(furthest_even_garden+2)/2;
        long long int full_gardens_odd_parity =  4 * (furthest_odd_garden + 1)/2*(furthest_odd_garden+1)/2;
        
        // INITIAL GARDEN
        total += single_garden_plots_visited(start, steps); 

        // GARDENS FULLY VISITED
        if (full_gardens_even_parity + full_gardens_odd_parity > 0)
            total += single_garden_plots_visited(start, steps) * full_gardens_even_parity + single_garden_plots_visited(start, steps+1) * full_gardens_odd_parity; 
        
        //PARTIAL SIDE GARDENS
        for (long long int garden_number{furthest_full_garden + 1}; garden_number<=furthest_full_garden + TAIL_GARDENS;garden_number++){
            if (garden_number < 2)
                continue;
            for (const auto& dir : DIRECTIONS){
                long long int max_visited = 0;
                for (const auto& extraction_point : {dir + dir.rotate_left(), dir + dir.rotate_right(), dir}){
                    long long int distance_to_garden = distance_from_to(start, unit_to_edge_pos(extraction_point)) + 1 + ((garden_number) - 2) * (plots.get_width()); // square garden
                    long long int steps_left = steps - distance_to_garden;
                    
                    Coord entry_point = {
                        dir.first != 0 ? -1 * extraction_point.first : extraction_point.first,
                        dir.second != 0 ? -1 * extraction_point.second : extraction_point.second
                        };
                    if (steps_left >= 0)
                            max_visited = std::max(max_visited,  single_garden_plots_visited(unit_to_edge_pos(entry_point), steps_left));
                }
                if (max_visited > 0)
                    total += max_visited;
            }
        }
        for (long long int garden_number{furthest_full_garden}; garden_number<=furthest_full_garden + TAIL_GARDENS;garden_number++){
            if (garden_number < 2)
                continue;
            for (const auto& corner : CORNERS){
                long long int distance_to_garden = distance_from_to(start, unit_to_edge_pos(corner)) + 2 + (garden_number - 2) * plots.get_width(); // square garden
                long long int steps_left = steps - distance_to_garden;
                long long int number_of_gardens = (garden_number - 1);
                if (steps_left >= 0)
                    total += number_of_gardens * single_garden_plots_visited(unit_to_edge_pos(corner*(-1)), steps_left);
            }
        }
        return total;
    }

    
    std::string garden_type(){
        if (plots.get_width()>16)
            return "big";
        return "small";
    }
private:
    long long int single_garden_plots_visited(Coord starting_pos, long long int steps){
        if (single_garden_plots_visited_cache.contains(starting_pos)){
            if (static_cast<size_t>(steps) >= single_garden_plots_visited_cache[starting_pos].size()){
                size_t id = (single_garden_plots_visited_cache[starting_pos].size()-1) - ((static_cast<size_t>(steps) - (single_garden_plots_visited_cache[starting_pos].size()-1)) % 2 == 0 ? 0 : 1);
                return single_garden_plots_visited_cache[starting_pos][id];
            }else{
                return single_garden_plots_visited_cache[starting_pos][static_cast<size_t>(steps)];
            }
        }
        single_garden_plots_visited_cache.emplace(starting_pos, std::vector<long long int>());
        std::unordered_set<Coord> prev{};
        std::unordered_set<Coord> current{starting_pos};
        long long int visited_prev = 0;
        long long int visited = 1;
        for(long long int step{0}; ; step++){
            single_garden_plots_visited_cache[starting_pos].push_back(visited);
            take_step(prev, current);
            if (current.size() == 0)
                break;
            
            std::swap(visited_prev, visited);
            visited += static_cast<long long int>(current.size());
        }
        return single_garden_plots_visited(starting_pos, steps);
    }

    
    void take_step(std::unordered_set<Coord>& prev, std::unordered_set<Coord>& current){
        std::unordered_set<Coord> next{};
        for (const auto& coord : current)
            for (auto new_coord : surrounding_plots(coord))
                if (!prev.contains(new_coord))
                    next.emplace(new_coord);
            
        
        prev = current;
        current = next;
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

    Coord unit_to_edge_pos(Coord corner){
        return Coord(
            (plots.get_width() - 1) *  (1 + corner.first)/2,
            (plots.get_height() - 1) *  (1 + corner.second)/2
        );
    }
    long long int distance_from_to(Coord starting_pos, Coord destination){
        if (distance_from_to_cache.contains({starting_pos, destination}))
            return distance_from_to_cache[{starting_pos, destination}];
        
        std::unordered_set<Coord> prev{};
        std::unordered_set<Coord> reached{starting_pos};
        long long int steps = 0;
        while(!reached.contains(destination)){
            steps++;
            take_step(prev, reached);
        }
        distance_from_to_cache.emplace(std::make_pair(std::make_pair(starting_pos, destination), steps));
        return steps;
    }
};

std::optional<std::string> solve(std::string_view input){
    Garden garden(input);
    int steps = garden.garden_type() == "big" ? STEPS_BIG_GARDEN : STEPS_SMALL_GARDEN;
    return std::to_string(garden.plots_visited(steps));
}
int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(21, 2, solve);
    if (argc > 1){
        solution.run("day_21_test_2.txt");
        long long int expected_result = (static_cast<long long int>(STEPS_SMALL_GARDEN) + static_cast<long long int>(1)) * (static_cast<long long int>(STEPS_SMALL_GARDEN) + static_cast<long long int>(1));
        std::cout<<" == "<<expected_result<<'\n';
        return 0;
        // return solution.test_run();
    }else{
        return solution.run();
    }
}
