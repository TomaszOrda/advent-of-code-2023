#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <cmath>


#define DIMENSIONS_TO_CONSIDER 3

constexpr double EPSILON = 0.00001;



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


class HailStone{
public:
    Coord3d position{};
    Coord3d velocity{};
    HailStone(std::string_view text){
        const auto parts = split(text, " @ ");
        const auto position_part = split(parts[0], ", ");
        const auto velocity_part = split(parts[1], ", ");
        position = Coord3d(svtod(position_part[0]), svtod(position_part[1]), svtod(position_part[2]));
        velocity = Coord3d(svtod(velocity_part[0]), svtod(velocity_part[1]), svtod(velocity_part[2]));
    }
    
};


class Hail{
    std::vector<HailStone> hailstones{};
public:
    Hail(std::string_view text){
        for (const auto& line : split_lines(text)){
            hailstones.push_back(HailStone(line));
        }
    }
    void perfect_rock_position(){
        std::cout<<std::fixed << std::setprecision(0);
        std::cout<<"I am very tired and even more irritaded by the C++ itself at this point.\nPartially as a joke and partially from the exhaustion;\n";
        std::cout<<"Paste into Python:\n\n\n";
        std::cout<<"from sympy import symbols, Eq, solve\n";
        std::cout<<"t0, t1, t2, v0, v1, v2, c0, c1, c2 = symbols('t0 t1 t2 v0 v1 v2 c0 c1 c2', integer=True) \n";
        
        for (size_t i{}; i<3; i++){
            for (size_t dim{}; dim<3; dim++){
                std::cout<<"eq"<<i*3+dim<<" = Eq("<<"t"<<i<<"*v"<<dim<<"+"<<"c"<<dim<<", "<<"t"<<i<<"*("<<(hailstones[i].velocity.at(dim))<<")+("<<(hailstones[i].position.at(dim))<<"))\n";
            }
        }
        std::cout<<"solutions = solve([eq0, eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8], (c0, c1, c2, t0, t1, t2, v0, v1, v2))\n";
        std::cout<<"print(solutions)\nprint(sum(solutions[0][0:3]))\n\n\n";
    }
private:
};

std::optional<std::string> solve(std::string_view input){
    Hail hail{input};
    hail.perfect_rock_position();
    return "";
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(24, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
