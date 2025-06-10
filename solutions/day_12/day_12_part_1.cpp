#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <vector>
#include <ranges>
#include <numeric>

class DamagedSprings{
    std::string springs;
    std::vector<int> groups;
public:
    DamagedSprings(std::string_view record)
        : springs {split(record, ' ')[0]}
        , groups {std::ranges::to<std::vector<int>>(split(split(record, ' ')[1], ',') | std::views::transform(svtoi))}
    {}
    int number_of_possible_arrangements(){
        return number_of_possible_arrangements_aux(springs, groups);
    }
private:
    int number_of_possible_arrangements_aux(std::string_view springs_slice, std::span<int> groups_slice){
        if (groups_slice.empty()){
            if (springs_slice.contains('#'))
                return 0;
            else
                return 1;
        }else if (springs_slice.length()==0)
            return 0;

        int result_skip = 0;
        int result_fit = 0;
        size_t group_size = static_cast<size_t>(groups_slice[0]);
        if (group_size > springs_slice.length())
            return 0;
        
        if (group_size == springs_slice.length()) {
            if(groups_slice.size()==1 &&  !springs_slice.substr(0, group_size).contains('.'))
                return  1;
            else
                return 0;
        }
        if (!springs_slice.substr(0, group_size).contains('.') && std::string(".?").contains(springs_slice[group_size]))
            result_fit = number_of_possible_arrangements_aux(
                springs_slice.substr(group_size + 1, springs_slice.length()-group_size-1), 
                groups_slice | std::views::drop(1));
        
        if (springs_slice[0] != '#')
            result_skip = number_of_possible_arrangements_aux(
                springs_slice.substr(1, springs_slice.length()-1), 
                groups_slice);

        return result_skip + result_fit;
    }
};

std::optional<std::string> solve(std::string_view input){
    auto condition_records = split_lines(input) | std::views::transform([](auto x){return DamagedSprings(x).number_of_possible_arrangements();});
    return std::to_string(std::accumulate(condition_records.begin(), condition_records.end(), 0));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(12, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
