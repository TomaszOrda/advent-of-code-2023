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
    Coord operator-(const Coord& other) const{
        return Coord(first - other.first, second - other.second);
    }
    Coord operator-() const{
        return Coord(-first, -second);
    }
    bool operator==(const Coord& other) const{
        return first == other.first && second == other.second;
    }
    Coord rotate_left() const{
        return Coord(second, -first);
    }
    Coord rotate_right() const{
        return rotate_left().rotate_left().rotate_left();
    }
};

class PipeSystem{
    Grid<char> pipe_grid;
    Coord start{};
public:
    PipeSystem(std::string_view pipe_system_map) : pipe_grid{pipe_system_map} {
        start = pipe_grid.find('S');
    }
    int area_enclosed(){
        int right_turns = 0;
        Grid<char> clasified_tiles(pipe_grid.get_width(), pipe_grid.get_height(), 'o');
        clasified_tiles.replace(start.first, start.second, 'p');
        Coord current = connects_to(start).value().first;
        Coord prev = start;
        Coord in_dir = current - start;
        do{
            clasified_tiles.replace(current.first, current.second, 'p');
            auto connected = connects_to(current).value();
            Coord next =
                connected.first == prev ? 
                connected.second : 
                connected.first;
            Coord out_dir = next - current;
            if (out_dir == in_dir.rotate_right()){
                right_turns+=1;
            }else if (out_dir == in_dir.rotate_left()){
                right_turns-=1;
            }
            
            for (const Coord& tile : {current + in_dir.rotate_left(), current + out_dir.rotate_left()})
                if (clasified_tiles.at(tile.first, tile.second) && clasified_tiles.at(tile.first, tile.second).value() == 'o')
                    clasified_tiles.replace(tile.first, tile.second, 'l');
            
            for (const Coord& tile : {current + in_dir.rotate_right(), current + out_dir.rotate_right()})
                if (clasified_tiles.at(tile.first, tile.second) && clasified_tiles.at(tile.first, tile.second).value() == 'o')
                    clasified_tiles.replace(tile.first, tile.second, 'r');

            prev = Coord(current);
            current = Coord(next);
            in_dir = out_dir;
        }while(current!=start);

        char inside_tile_classifier{right_turns >0 ? 'r' : 'l'};

        std::deque<Coord> stack {};
        for (int x{0}; x<clasified_tiles.get_width(); x++){
            for (int y{0}; y<clasified_tiles.get_height(); y++)
                if (clasified_tiles.at(x,y).value() == inside_tile_classifier)
                    stack.push_back(Coord(x,y));
        }

        while(!stack.empty()){
            Coord tile = stack.front();
            if (clasified_tiles.at(tile.first, tile.second).value() != inside_tile_classifier){
                std::cout<<"!"<<clasified_tiles.at(tile.first, tile.second).value();
            }
            for(const auto& neighbour : surrounding(tile))
                if (clasified_tiles.at(neighbour.first, neighbour.second).value() == 'o' ){
                    clasified_tiles.replace(neighbour.first, neighbour.second, inside_tile_classifier);
                    stack.push_back(neighbour);
                }
            
            stack.pop_front();
        }

        int area = 0;
        for (int x{0}; x<clasified_tiles.get_width(); x++)
            for (int y{0}; y<clasified_tiles.get_height(); y++)
                if (clasified_tiles.at(x, y).value() == inside_tile_classifier)
                    area+=1;
        return area;
    }
private:
    std::optional<std::pair<Coord, Coord>> connects_to(Coord pos){
        if (! pipe_grid.at(pos.first, pos.second))
            return std::nullopt;
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
    std::vector<Coord> surrounding(Coord pos){
        std::vector<Coord> result{};
        for (const auto& dir : DIRECTIONS){
            Coord tile_pos = pos + dir;
            auto tile = pipe_grid.at(tile_pos.first, tile_pos.second);
            if (tile){
                result.push_back(tile_pos);
            }
        }
        return result;
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(PipeSystem(input).area_enclosed());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(10, 2, solve);
    if (argc > 1){
        solution.run("day_10_test_3.txt");
        std::cout<<'\n';
        solution.run("day_10_test_4.txt");
        std::cout<<'\n';
        return solution.run("day_10_test_5.txt");
    }else{
        return solution.run();
    }
}
