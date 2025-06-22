#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <unordered_set>
#include <set>

//Assume that bricks expand towards bigger indexes
//It is clear that the grid size is supposed to be 10
//Oversized grids have no impact on correctness

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
    int& operator[](size_t index){//not tested
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

class Brick{
    Coord3d start{};
    Coord3d end{};
public:
    bool can_be_desintegrated{true};
    Brick(){};
    Brick(std::string_view text){
        auto parts = split(text, '~');
        auto start_parts = split(parts[0], ',');
        start = Coord3d(svtoi(start_parts[0]), svtoi(start_parts[1]), svtoi(start_parts[2]));
        auto end_parts = split(parts[1], ',');
        end = Coord3d(svtoi(end_parts[0]), svtoi(end_parts[1]), svtoi(end_parts[2]));
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
    void set_supporting(){
        can_be_desintegrated = false;
    }
    friend std::ostream& operator<<(std::ostream& os, const Brick& brick);
};
std::ostream& operator<<(std::ostream& os, const Brick& brick) {
    os<<brick.start.x<<','<<brick.start.y<<','<<brick.start.z<<'~'<<brick.end.x<<','<<brick.end.y<<','<<brick.end.z;
    return os;
}

struct CompareBricksReferences {
    bool operator()(const std::reference_wrapper<Brick>& a,
                    const std::reference_wrapper<Brick>& b) const {
        return &a.get() < &b.get();
    }
};


class LandingZone{
    Brick ground_brick{Brick("0,0,0~0,0,0")};
    std::vector<Brick> bricks{};
    Grid<std::reference_wrapper<Brick>> top_brick;
    Grid<int> top_brick_height;
public:
    LandingZone() 
        : top_brick{Grid<std::reference_wrapper<Brick>>(GRID_SIZE, GRID_SIZE,std::ref(ground_brick))}
        , top_brick_height{Grid<int>(GRID_SIZE, GRID_SIZE, 0)}
    {}
    void load_brick(std::string_view brick_text){
        bricks.push_back(Brick(brick_text));
    }
    void simulate(){
        std::sort(bricks.begin(), bricks.end());
        for (auto& brick : bricks){
            drop_brick(brick);

            update_supporting_bricks(brick);
            
            update_new_top_brick(brick);
        }
    }
    long long int get_disintegrable_bricks() const{
        return std::count_if(bricks.begin(), bricks.end(), [](auto x){return x.can_be_desintegrated;});
    }
private:
    void drop_brick(Brick& brick){
        int z_after_fall = 1;
        for (const auto& coord : brick.get_top_view_cubes())
            z_after_fall = std::max(z_after_fall, top_brick_height.at(coord).value()+1);
        
        brick.drop_to(z_after_fall);
    }
    void update_supporting_bricks(const Brick& brick){
        int brick_z = brick.get_base_height();

        std::set<std::reference_wrapper<Brick>, CompareBricksReferences> supporting_bricks{}; 
        for (const auto& coord : brick.get_top_view_cubes()){
            if (brick_z-1 == top_brick_height.at(coord).value()){
                supporting_bricks.emplace(top_brick.at(coord).value());
            }
        }

        if (supporting_bricks.size() == 1)
            (*supporting_bricks.begin()).get().set_supporting();
        
    }
    void update_new_top_brick(Brick& brick){
        for (const auto& cube : brick.get_cubes()){
            top_brick.replace(Coord(cube.x, cube.y), brick);
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
    return std::to_string(landing_zone.get_disintegrable_bricks());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(22, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
