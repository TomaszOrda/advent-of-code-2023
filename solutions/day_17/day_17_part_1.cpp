#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <deque>
#include <algorithm>


class CrucibleState{
public:
    Coord pos{};
    Coord dir{};
    int traveled_straight{};
    int heat_loss{};
    CrucibleState(){}
    CrucibleState(Coord pos, Coord dir, int traveled_straight, int heat_loss)
        : pos{pos}
        , dir{dir}
        , traveled_straight{traveled_straight}
        , heat_loss{heat_loss}
    {}
    bool can_go_straight(){
        return traveled_straight != 3;
    }
    CrucibleState go_straight(){
        return {pos+dir, dir, traveled_straight+1, heat_loss};
    }
    CrucibleState turn_right(){
        Coord new_dir = dir.rotate_right();
        return {pos+new_dir, new_dir, 1, heat_loss};
    }
    CrucibleState turn_left(){
        Coord new_dir = dir.rotate_left();
        return {pos+new_dir, new_dir, 1, heat_loss};
    }
    bool operator<=(const CrucibleState& other) const{
        return (heat_loss <= other.heat_loss && dir == other.dir && traveled_straight <= other.traveled_straight) || heat_loss < other.heat_loss - 18;
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
    , start_1{CrucibleState({0,0}, Coord::East,  0, -digit_to_int(traffic.at(0,0).value()))}
    , start_2{CrucibleState({0,0}, Coord::South, 0, -digit_to_int(traffic.at(0,0).value()))}
    , end{traffic.get_width()-1, traffic.get_height()-1}
    {}
    int min_heat_loss(){
        Grid<std::vector<CrucibleState>> min_heat_loss_states{traffic.get_width(), traffic.get_height(), {}};
        std::deque<CrucibleState> to_visit{};
        to_visit.push_back(start_1);
        to_visit.push_back(start_2);
        while(!to_visit.empty()){
            CrucibleState state = to_visit.front();
            Coord pos = state.pos;
            to_visit.pop_front();
            if (!min_heat_loss_states.at(pos))
                continue;
            
            state.heat_loss += digit_to_int(traffic.at(state.pos).value());
            auto states = min_heat_loss_states.at(pos).value();

            if (std::any_of(states.begin(), states.end(), [state](const auto& s){return s<=state;}))
                continue;
            
            min_heat_loss_states.update(pos, [state](std::vector<CrucibleState>& x){
                x.erase(std::remove_if(x.begin(), x.end(), [state](auto s){return state<=s;}), x.end());
                x.push_back(state);});
            to_visit.push_back(state.turn_right());
            to_visit.push_back(state.turn_left());
            if (state.can_go_straight())
                to_visit.push_back(state.go_straight());

        }
        int min = INT_MAX;
        auto end_states = min_heat_loss_states.at(end).value();
        for (const auto& state : end_states)
            min = std::min(min, state.heat_loss);

        return min;
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(TrafficMap(input).min_heat_loss());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(17, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
