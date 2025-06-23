#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>

#define FOREST '#'
#define PATH '.'
#define SLOPES std::string("^>v<")

#define DIRECTIONS {Coord::North, Coord::East, Coord::South, Coord::West}

Coord slope_to_dir(char slope){
    switch (slope)
    {
    case '^':
        return Coord::North;
    case '>':
        return Coord::East;
    case 'v':
        return Coord::South;
    case '<':
        return Coord::West;
    default:
        throw std::runtime_error("Unknown slope");
    }
}

struct MazeHikeState{
    Coord current_pos;
    uint64_t visited_intersections;
    int current_length;
};
struct InterIntersectionHikeState{
    Coord prev_pos;
    Coord current_pos;
    int current_length;
};

class Maze{
    Grid<char> map;
    std::set<Coord> intersections{};
    std::map<Coord, std::unordered_map<Coord, int>> distance_between_intersections{};
    std::map<Coord, size_t> intersection_to_index{};
    Coord start{};
    Coord end{};
    //For the ease of calculations, we will consider starting at second position
    const static int enter_exit_distance{2};
public:
    Maze(std::string_view text) : map{Grid<char>(text)}{
        start = Coord(1,1);
        intersections.emplace(start);
        map.replace(start + Coord::North, FOREST);
        end = Coord(map.get_width()-2, map.get_height()-2);
        intersections.emplace(end);
        map.replace(end + Coord::South, FOREST);
        for (int x{}; x<map.get_width(); x++)
            for (int y{}; y<map.get_height(); y++)
                if (map.at(x,y).value() != FOREST && surrounding_paths(Coord(x,y)).size()>2){
                    intersections.emplace(Coord(x,y));
                    distance_between_intersections.emplace(Coord(x,y), std::unordered_map<Coord, int>());
                }

        int intersection_id =0;
        for (const auto& intersection : intersections)
            intersection_to_index.emplace(intersection, intersection_id++);

        for (int x{}; x<map.get_width(); x++)
            for (int y{}; y<map.get_height(); y++)
                if (SLOPES.contains(map.at(x,y).value()))
                    map.replace(x, y, PATH);

        calculate_intersections_distances();
    }
    int longest_hike_length(){
        // return longest_hike_length_recursive(start, 0);
        return longest_hike_length_iterative();
    }
    
private:
    int longest_hike_length_iterative(){
        int result = 0;
        std::vector<MazeHikeState> stack;
        stack.push_back(MazeHikeState{start, 0, 0});
        while(!stack.empty()){
            const auto state = stack.back();
            stack.pop_back();
            if (state.current_pos == end){
                result = std::max(result, state.current_length);
                continue;
            }
            const size_t current_pos_id = intersection_to_index[state.current_pos];
            const uint64_t new_visited = state.visited_intersections | (1ULL << current_pos_id);
            for (const auto& intersection : distance_between_intersections[state.current_pos]){
                if (!(state.visited_intersections & (1ULL << intersection_to_index[intersection.first]))){
                    stack.push_back(MazeHikeState{intersection.first, new_visited, state.current_length + intersection.second});
                }
            }
        }
        return result + enter_exit_distance;
    }
    int longest_hike_length_recursive(Coord starting_pos, uint64_t visited_intersections){
        if (starting_pos == end){
            return enter_exit_distance;
        }
        
        size_t starting_pos_id = intersection_to_index[starting_pos];
        visited_intersections |= (1ULL << starting_pos_id);
        int result = 0;
        for (const auto& intersection : distance_between_intersections[starting_pos]){
            if (!(visited_intersections & (1ULL << intersection_to_index[intersection.first]))){
                result = std::max(result, intersection.second + longest_hike_length_recursive(intersection.first, visited_intersections));                
            }
        }
        
        return result;
    }
    void calculate_intersections_distances(){
        for (const Coord& intersection : intersections){
            std::vector<InterIntersectionHikeState> stack{InterIntersectionHikeState{intersection, intersection, 0}};
            while(!stack.empty()){
                Coord current = stack.back().current_pos;
                Coord prev = stack.back().prev_pos;
                int current_distance = stack.back().current_length;
                stack.pop_back();

                std::unordered_set<Coord> neighbours{};
                if (SLOPES.contains(map.at(current).value())){
                    neighbours.emplace(current + slope_to_dir(map.at(current).value()));
                }else{
                    neighbours = surrounding_paths(current);
                }

                for (const Coord& pos : neighbours){
                    if (pos == prev)
                        continue;
                    if (intersections.contains(pos)){
                        distance_between_intersections[intersection].emplace(pos, current_distance + 1);
                    }else{
                        stack.push_back(InterIntersectionHikeState{current, pos, current_distance+1});
                    }
                }
            }
        }
    }
    std::unordered_set<Coord> surrounding_paths(Coord pos){
        std::unordered_set<Coord> result;
        for (const auto& dir : DIRECTIONS){
            Coord new_pos = pos + dir;
            if (map.at(new_pos).value()!=FOREST)
                result.emplace(new_pos);
        }
        return result;
    }
};

std::optional<std::string> solve(std::string_view input){
    Maze maze{input};
    return std::to_string(maze.longest_hike_length());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(23, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
