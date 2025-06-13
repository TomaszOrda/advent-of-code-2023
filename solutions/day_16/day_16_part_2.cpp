#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>


class VisitedTiles : private Grid<std::array<bool, 4>>{
private:
    // int energized_tiles_count{};
public:
    VisitedTiles(int width, int height) : Grid<std::array<bool, 4>>(width, height, {false, false, false, false}){}
    int count(){//This could be done eagerly, but the time reduction appears to be marginal
        //return energized_tiles_count;
        int total = 0;
        for (int x{}; x<get_width(); x++){
            for (int y{}; y<this->get_height(); y++){
                auto arr = this->at(x,y).value();
                total += arr[0] || arr[1] || arr[2] || arr[3];
            }       
        }
        return total;
    }
    void set_visited(Coord c, Coord direction){
        auto arr = this->at(c).value();
        // if (!arr[0] && !arr[1] && !arr[2] && !arr[3])
        //     energized_tiles_count++;
        arr[direction_to_id(direction)] = true;
        this->replace(c, arr);
    }
    bool visited(Coord c, Coord direction){
        return this->at(c).value()[direction_to_id(direction)]; 
    }
private:
    size_t direction_to_id(Coord direction){
        if (direction == Coord::North){
            return 0;
        }else if (direction == Coord::South){
            return 1;
        }else if (direction == Coord::East){
            return 2;
        }else if (direction == Coord::West){
            return 3;
        }else{
            throw std::runtime_error("Unknown direction");
        }
    }
};

class Contraption{
    Grid<char> grid;
    VisitedTiles visited_tiles;
public:
    Contraption(std::string_view text) 
    : grid{text}
    , visited_tiles(grid.get_width(), grid.get_height())
    {}
    void reset_tiles(){
        visited_tiles = VisitedTiles(grid.get_width(), grid.get_height());
    }
    void energize_tiles(Coord start_coord = Coord(0,0), Coord start_direction = Coord::East){
        std::vector<std::pair<Coord, Coord>> beams{};
        beams.push_back({start_coord, start_direction});
        while(!beams.empty()){
            Coord coord = beams.back().first;
            Coord direction = beams.back().second;
            beams.pop_back();
            
            if (grid.at(coord) && !visited_tiles.visited(coord, direction)){
                visited_tiles.set_visited(coord, direction);
                char tile = grid.at(coord).value();
                if (is_splitter(tile)){
                    for (const Coord& next_dir : split_direction(tile, direction)){
                        beams.push_back({coord + next_dir, next_dir});
                    }
                }else if (is_mirror(tile)){
                    Coord next_dir = mirror_bounce_direction(tile, direction);
                    beams.push_back({coord + next_dir, next_dir});
                }else if (tile =='.') {
                    beams.push_back({coord + direction, direction});
                }else {
                    throw std::runtime_error("Unknown tile");
                }
            }
        }
    }
    int number_of_energized_tiles(){
        return visited_tiles.count();
    }
    int max_number_of_energized_tiles(){
        int max = 0;
        for(int x{}; x<grid.get_width();x++){
            energize_tiles({x,0}, Coord::South);
            max = std::max(max, number_of_energized_tiles());
            reset_tiles();
            energize_tiles({x,grid.get_height()-1}, Coord::North);
            max = std::max(max, number_of_energized_tiles());
            reset_tiles();
        }
        for(int y{}; y<grid.get_height();y++){
            energize_tiles({0,y}, Coord::East);
            max = std::max(max, number_of_energized_tiles());
            reset_tiles();
            energize_tiles({grid.get_width()-1,y}, Coord::West);
            max = std::max(max, number_of_energized_tiles());
            reset_tiles();
        }
        return max;
    }
private:
    std::vector<Coord> split_direction(char splitter, Coord direction){
        std::vector<Coord> result{};
        if (splitter=='-'){
            result = {Coord::East, Coord::West};
        }else if (splitter=='|'){
            result = {Coord::South, Coord::North};
        }else{
            throw std::runtime_error("Unknown splitter");
        }
        if (std::find(result.begin(), result.end(), direction) == result.end()){
            return result;
        }else{
            return {direction};
        }
    }
    Coord mirror_bounce_direction(char mirror, Coord direction){
        if (direction == Coord::East || direction == Coord::West){
            if (mirror == '\\'){
                return direction.rotate_right();
            }else if (mirror == '/'){
                return direction.rotate_left();
            }else{
                throw std::runtime_error("Unknown mirror");
            }
        }else if (direction == Coord::South || direction == Coord::North){
            if (mirror == '\\'){
                return direction.rotate_left();
            }else if (mirror == '/'){
                return direction.rotate_right();
            }else{
                throw std::runtime_error("Unknown mirror");
            }
        }else {
            throw std::runtime_error("Unknown direction");
        }
    }
    bool is_splitter(char c){
        return c=='-' || c=='|';
    }
    bool is_mirror(char c){
        return c=='/' || c=='\\';
    }
};

std::optional<std::string> solve(std::string_view input){
    Contraption contraption(input);
    return std::to_string(contraption.max_number_of_energized_tiles());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(16, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
