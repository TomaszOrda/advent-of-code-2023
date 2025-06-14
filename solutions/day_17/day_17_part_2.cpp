#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <deque>
#include <algorithm>

#define MIN_STRAIGHT_ROUTE 4
#define MAX_STRAIGHT_ROUTE 10

class CrucibleState{
public:
    Coord pos{};
    Coord dir{};
    int heat_loss{};
    CrucibleState(){}
    CrucibleState(Coord pos, Coord dir, int heat_loss)
        : pos{pos}
        , dir{dir}
        , heat_loss{heat_loss}
    {}
    void go_straight(const Grid<char>& traffic){
        pos = pos+dir;
        heat_loss = traffic.at(pos) ? heat_loss + digit_to_int(traffic.at(pos).value()) : INT_MAX;
    }
    CrucibleState turn_right(){
        return {pos, dir.rotate_right(), heat_loss};
    }
    CrucibleState turn_left(){
        return {pos, dir.rotate_left(), heat_loss};
    }
};


class TrafficMap{
    Grid<char> traffic;
    CrucibleState start_1;
    CrucibleState start_2;
    Coord end;
public:
    TrafficMap(std::string_view text) 
    : traffic{text}
    , start_1{CrucibleState({0,0}, Coord::East,  0)}
    , start_2{CrucibleState({0,0}, Coord::South, 0)}
    , end{traffic.get_width()-1, traffic.get_height()-1}
    {}
    int min_heat_loss(){
        Grid<int> min_heat_loss_horizontal{traffic.get_width(), traffic.get_height(), INT_MAX};
        Grid<int> min_heat_loss_vertical{traffic.get_width(), traffic.get_height(), INT_MAX};

        int max_heat_loss = 0;
        Coord naive_route{0,0};
        while(naive_route != end){
            if (traffic.get_width() - naive_route.first > traffic.get_height() - naive_route.second){
                naive_route = naive_route + Coord::East;
            }else{
                naive_route = naive_route + Coord::South;
            }
            max_heat_loss += traffic.at(naive_route).value();
        }

        std::deque<CrucibleState> to_visit{};
        to_visit.push_back(start_1);
        to_visit.push_back(start_2);
        while(!to_visit.empty()){
            CrucibleState state = to_visit.front();
            Coord pos = state.pos;
            to_visit.pop_front();

            Grid<int>& min_heat_loss_grid = state.dir.first == 0 ? min_heat_loss_vertical : min_heat_loss_horizontal;

            if (!traffic.at(pos))
                continue;
            if (min_heat_loss_grid.at(pos).value() <= state.heat_loss)
                continue;
            if (state.heat_loss + (end - state.pos).manhattan_distance() > max_heat_loss)
                continue;

            min_heat_loss_grid.replace(pos, state.heat_loss);

            auto state_right = state.turn_right();
            auto state_left = state.turn_left();
            for (int straight{}; straight<MIN_STRAIGHT_ROUTE-1; straight++){
                state_right.go_straight(traffic);
                state_left.go_straight(traffic);
            }
            for (int straight{MIN_STRAIGHT_ROUTE-1}; straight<MAX_STRAIGHT_ROUTE; straight++){
                state_right.go_straight(traffic);
                state_left.go_straight(traffic);
    
                to_visit.push_back(state_right);
                to_visit.push_back(state_left);
            }

        }
        return std::min(min_heat_loss_vertical.at(end).value(), min_heat_loss_horizontal.at(end).value());
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(TrafficMap(input).min_heat_loss());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(17, 2, solve);
    if (argc > 1){
        solution.test_run();
        std::cout<<'\n';
        return solution.run("day_17_test_2.txt");
    }else{
        return solution.run();
    }
}
