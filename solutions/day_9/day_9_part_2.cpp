#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <ranges>
#include <numeric>
#include <vector>

class ValueReport{
    std::vector<int> sequence;
public:
    ValueReport(std::vector<int> sequence) : sequence{sequence}
    {}
    ValueReport(std::string_view sequence_record) 
        : sequence{std::ranges::to<std::vector<int>>(split(sequence_record, ' ') | std::views::transform(svtoi))}
    {}
    bool all_zeros() const{
        return std::all_of(sequence.begin(), sequence.end(), [](auto x){return x==0;});
    }
    ValueReport next() const{
        std::vector<int> result{};
        for (size_t i{1}; i<sequence.size(); i++) {
            result.push_back(sequence[i] - sequence[i-1]);
        }
        return ValueReport(result);
    }
    int prediction() const{
        if (all_zeros()){
            return 0;
        }
        return sequence.back() + next().prediction();
    }
    int extrapolate_backwards() const{
        if (all_zeros()){
            return 0;
        }
        return sequence[0] - next().extrapolate_backwards();
    }
};

class OasisReport{
    std::vector<ValueReport> reports{};
public:
    OasisReport(std::string_view record){
        for (const auto& line : split_lines(record)){
            reports.push_back(ValueReport(line));
        }
    }
    int sum_of_predictions(){
        return std::accumulate(
            reports.begin(), 
            reports.end(), 
            0, 
            [](int a, ValueReport& b){return a + b.prediction();});
    }
    int sum_of_extrapolated_values(){
        return std::accumulate(
            reports.begin(), 
            reports.end(), 
            0, 
            [](int a, ValueReport& b){return a + b.extrapolate_backwards();});
    }
};

std::optional<std::string> solve(std::string_view input){
    return std::to_string(OasisReport(input).sum_of_extrapolated_values());
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(9, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
