#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <cmath>


#define DIMENSIONS_TO_CONSIDER 3

constexpr double EPSILON = 0.00001;

constexpr double MIN_POS = 200000000000000;
constexpr double MAX_POS = 400000000000000;

constexpr double MIN_POS_TEST = 7;
constexpr double MAX_POS_TEST = 27;


class Coord3d{
public:
    double x;
    double y;
    double z;
    constexpr Coord3d(): x{0}, y{0}, z{0} {};
    constexpr Coord3d(double x, double y, double z) : x{x}, y{y}, z{z}{};
    Coord3d operator-(const Coord3d& other) const{
        return Coord3d(x - other.at(0), y - other.at(1), z - other.at(2));
    };
    Coord3d operator+(const Coord3d& other) const{
        return Coord3d(x + other.at(0), y + other.at(1), z + other.at(2));
    };
    Coord3d operator/(const double& scalar) const{
        return Coord3d(x/scalar, y/scalar, z/scalar);
    };
    bool operator==(const Coord3d& other) const{
        return x == other.at(0) && y == other.at(1) && z == other.at(2);
    };
    double manhattan_distance() const{
        return std::abs(at(0)) + std::abs(at(1)) + std::abs(at(2));
    }
    double at(size_t index) const{
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
    double& operator[](size_t index){
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

struct CoordDouble{
    double first;
    double second;
};


struct Path2d{
    double a;
    double b;
};

class HailStone{
    Coord3d position{};
    Coord3d velocity{};
public:
    bool smashed{false};
    HailStone(std::string_view text){
        const auto parts = split(text, " @ ");
        const auto position_part = split(parts[0], ", ");
        const auto velocity_part = split(parts[1], ", ");
        position = Coord3d(svtod(position_part[0]), svtod(position_part[1]), svtod(position_part[2]));
        velocity = Coord3d(svtod(velocity_part[0]), svtod(velocity_part[1]), svtod(velocity_part[2]));
    }
    Path2d path(){
        return Path2d(velocity.at(1)/velocity.at(0), position.at(1) - velocity.at(1)/velocity.at(0)*position.at(0));
    }
    bool pos2d_in_future(CoordDouble pos){
        return (pos.first - position.at(0))/velocity.at(0) > -EPSILON;
    }
    
};

struct Collision{
    size_t hailstone_1_id;
    size_t hailstone_2_id;
    CoordDouble intersection;
};


class Hail{
    std::vector<HailStone> hailstones{};
public:
    Hail(std::string_view text){
        for (const auto& line : split_lines(text)){
            hailstones.push_back(HailStone(line));
        }
    }
    int hailstones_collisions_within(double min_pos, double max_pos){
        std::vector<Collision> collisions{};
        for (size_t h1{}; h1<hailstones.size(); h1++)
            for (size_t h2{h1+1}; h2<hailstones.size(); h2++){
                auto result = paths_intersection(hailstones[h1].path(), hailstones[h2].path());
                if (!result){
                    // std::cout<<"PARALLEL\n";
                    continue;
                }
                if (!hailstones[h1].pos2d_in_future(result.value())){
                    // std::cout<<"A in past\n";
                    continue;
                }
                if (!hailstones[h2].pos2d_in_future(result.value())){
                    // std::cout<<"B in past\n";
                    continue;
                }
                if (!pos2d_within(result.value(), min_pos, max_pos)){
                    // std::cout<<"outside\n";
                    continue;
                }
                collisions.push_back(Collision{h1, h2, result.value()});
                
            }
        return static_cast<int>(collisions.size());
    }
    std::string hail_type(){
        return hailstones.size() < 16 ? std::string("test") : std::string("normal");
    }
private:
    std::optional<CoordDouble> paths_intersection(const Path2d& path1, const Path2d& path2) const{
        if (std::abs(path1.a - path2.a) < EPSILON){
            return std::nullopt;
        }
        double result_x = (path2.b - path1.b)/(path1.a - path2.a);
        double result_y = path1.a * result_x + path1.b;
        return CoordDouble(result_x, result_y);
    }
    bool pos2d_within(CoordDouble pos, double min, double max){
        return min <= pos.first && pos.first <= max && min <= pos.second && pos.second <= max; 
    }
};

std::optional<std::string> solve(std::string_view input){
    Hail hail{input};
    double min_pos = hail.hail_type()==std::string("test") ? MIN_POS_TEST : MIN_POS;
    double max_pos = hail.hail_type()==std::string("test") ? MAX_POS_TEST : MAX_POS;
    return std::to_string(hail.hailstones_collisions_within(min_pos, max_pos));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(24, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
