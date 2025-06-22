#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_set>
#include <set>

//Assume that bricks expand towards bigger indexes
//It is clear that the grid size is supposed to be 10
//Oversized grids have no impact on correctness
//There is a lot of room for optimization, but the solution works fast enough right now

#define GRID_SIZE 10

class Coord3d{
public:
    int x;
    int y;
    int z;
    constexpr Coord3d(): x{0}, y{0}, z{0} {};
    constexpr Coord3d(int x, int y, int z) : x{x}, y{y}, z{z}{};
    Coord3d operator-(const Coord3d& other) const{
        return Coord3d(x - other.at(0), y - other.at(1), z - other.at(2));
    };
    Coord3d operator+(const Coord3d& other) const{
        return Coord3d(x + other.at(0), y + other.at(1), z + other.at(2));
    };
    Coord3d operator/(const int& scalar) const{
        return Coord3d(x/scalar, y/scalar, z/scalar);
    };
    bool operator==(const Coord3d& other) const{
        return x == other.at(0) && y == other.at(1) && z == other.at(2);
    };
    int manhattan_distance() const{
        return abs(at(0)) + abs(at(1)) + abs(at(2));
    }
    int at(size_t index) const{
        switch (index)
        {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            throw std::out_of_range("Index out of range");
            break;
        }
    }
};

class Brick;

struct CompareBricksReferences {
    bool operator()(const std::reference_wrapper<Brick>& a,
                    const std::reference_wrapper<Brick>& b) const {
        return &a.get() < &b.get();
    }
};

class Brick{
    Coord3d start{};
    Coord3d end{};
public:
    size_t id{};
    Brick(){};
    Brick(std::string_view text){
        auto parts = split(text, '~');
        auto start_parts = split(parts[0], ',');
        start = Coord3d(svtoi(start_parts[0]), svtoi(start_parts[1]), svtoi(start_parts[2]));
        auto end_parts = split(parts[1], ',');
        end = Coord3d(svtoi(end_parts[0]), svtoi(end_parts[1]), svtoi(end_parts[2]));
    }
    void set_id(size_t id_number){
        id = id_number;
    }
    std::vector<Coord3d> get_cubes() const{
        std::vector<Coord3d> result{};
        if ((end-start).manhattan_distance()>0){
            Coord3d dir = (end-start)/(end-start).manhattan_distance();
            for (Coord3d cube{start};cube!=end;cube=cube+dir){
                result.push_back(cube);
            }
        }
        result.push_back(end);
        return result;
    }
    std::unordered_set<Coord> get_top_view_cubes() const{
        std::unordered_set<Coord> result{};
        if ((end-start).manhattan_distance()>0){
            Coord3d dir = (end-start)/(end-start).manhattan_distance();
            for (Coord3d cube{start};cube!=end;cube=cube+dir){
                result.emplace(Coord(cube.x, cube.y));
            }
        }
        result.emplace(Coord(end.x, end.y));
        return result;
    }
    int get_base_height() const{
        return std::min(start.z, end.z);
    }
    void drop_to(int new_z){
        int drop_amount = start.z - new_z;
        start.z -= drop_amount;
        end.z -= drop_amount;
    }
    bool operator<=(const Brick& other) const{
        return start.z <= other.start.z;
    }
    bool operator==(const Brick& other) const{
        return start == other.start && end == other.end;
    }
    bool operator<(const Brick& other) const{
        return start.z < other.start.z;
    }
    friend std::ostream& operator<<(std::ostream& os, const Brick& brick);
};
std::ostream& operator<<(std::ostream& os, const Brick& brick) {
    os<<brick.start.x<<','<<brick.start.y<<','<<brick.start.z<<'~'<<brick.end.x<<','<<brick.end.y<<','<<brick.end.z;
    return os;
}




class LandingZone{
    std::vector<Brick> bricks{};
    std::vector<std::set<size_t>> is_supporting{};
    std::vector<std::set<size_t>> is_supported_by{};
    Grid<size_t> top_brick;
    Grid<int> top_brick_height;
public:
    LandingZone() 
        : top_brick{Grid<size_t>(GRID_SIZE, GRID_SIZE,SIZE_MAX)}
        , top_brick_height{Grid<int>(GRID_SIZE, GRID_SIZE, 0)}
    {}
    void load_brick(std::string_view brick_text){
        bricks.push_back(Brick(brick_text));
    }
    void simulate(){
        std::sort(bricks.begin(), bricks.end());
        for(size_t i{}; i<bricks.size(); i++){
            Brick& brick = bricks[i];
            brick.set_id(i);
            is_supporting.push_back(std::set<size_t>());
            is_supported_by.push_back(std::set<size_t>());
            drop_brick(brick);

            update_supporting_bricks(brick);
            
            update_new_top_brick(brick);
        }
    }
    long long int total_fallen_bricks(){
        long long int total{};
        for (const auto& brick : bricks)
            total += bricks_to_fall(brick);
        
        return total;
    }
private:
    int bricks_to_fall(const Brick& brick_to_remove){
        size_t remove_id = brick_to_remove.id;
        std::set<size_t> fallen_bricks{remove_id};
        std::vector<size_t> bricks_about_to_fall{};
        bricks_about_to_fall.insert(bricks_about_to_fall.end(), is_supported_by[remove_id].begin(), is_supported_by[remove_id].end());
        while(!bricks_about_to_fall.empty()){
            size_t brick_id = bricks_about_to_fall.back();
            bricks_about_to_fall.pop_back();

            if (std::includes(fallen_bricks.begin(), fallen_bricks.end(), is_supporting[brick_id].begin(), is_supporting[brick_id].end())){
                fallen_bricks.emplace(brick_id);
                for (auto new_brick_id : is_supported_by[brick_id]){
                    bricks_about_to_fall.push_back(new_brick_id);
                }
            }
        }
        return static_cast<int>(fallen_bricks.size()) - 1;
    }
    void drop_brick(Brick& brick){
        int z_after_fall = 1;
        for (const auto& coord : brick.get_top_view_cubes()){
            z_after_fall = std::max(z_after_fall, top_brick_height.at(coord).value()+1);
        }
        brick.drop_to(z_after_fall);
    }
    void update_supporting_bricks(Brick& brick){
        int brick_z = brick.get_base_height();

        //Hashmap might or might not be faster. But even if, its marginal.
        for (const auto& coord : brick.get_top_view_cubes()){
            if (brick_z-1 == top_brick_height.at(coord).value()){
                size_t supporting_id = top_brick.at(coord).value();
                if (supporting_id == SIZE_MAX)
                    continue;
                size_t supported_id = brick.id;
                is_supporting[supported_id].emplace(supporting_id);
                is_supported_by[supporting_id].emplace(supported_id);
            }
        }
    }
    void update_new_top_brick(Brick& brick){
        for (const auto& cube : brick.get_cubes()){
            top_brick.replace(Coord(cube.x, cube.y), brick.id);
            top_brick_height.replace(Coord(cube.x, cube.y), cube.z);
        }
    }
};

std::optional<std::string> solve(std::string_view input){
    LandingZone landing_zone{};
    for (const auto& line : split_lines(input)){
        landing_zone.load_brick(line);
    }
    landing_zone.simulate();
    return std::to_string(landing_zone.total_fallen_bricks());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(22, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
