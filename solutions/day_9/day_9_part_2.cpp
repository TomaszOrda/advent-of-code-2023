#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <ranges>
#include <numeric>
#include <vector>

class Sequence{
    std::vector<int> sequence;
public:
    Sequence(std::vector<int> sequence) : sequence{sequence}
    {}
    Sequence(std::string_view sequence_record) 
        : sequence{std::ranges::to<std::vector<int>>(split(sequence_record, ' ') | std::views::transform(svtoi))}
    {}
    bool all_zero() const{
        return std::all_of(sequence.begin(), sequence.end(), [](auto x){return x==0;});
    }
    Sequence next(){
        std::vector<int> result{};
        for (size_t i{1}; i<sequence.size(); i++) {
            result.push_back(sequence[i] - sequence[i-1]);
        }
        return Sequence(result);
    }
    int prediction(int next_sequence_prediction) const{
        return sequence.back() + next_sequence_prediction;
    }
    int extrapolate_backwards(int next_sequence_extrapolation) const{
        return sequence[0] - next_sequence_extrapolation;
    }
};

class OasisValueReport{
    std::vector<Sequence> sequences;
public:
    OasisValueReport(std::string_view record) : sequences{std::vector<Sequence>{Sequence(record)}}
    {}
    int prediction(){
        while(!sequences.back().all_zero()){
            sequences.push_back(
                sequences.back().next()
            );
        }
        int current_sequence_prediction = 0;
        for (const auto& sequence : sequences | std::views::reverse | std::views::drop(1) ){
            current_sequence_prediction = sequence.prediction(current_sequence_prediction);
        }
        return current_sequence_prediction;
    }
    int extrapolate_backwards(){
        while(!sequences.back().all_zero()){
            sequences.push_back(
                sequences.back().next()
            );
        }
        int current_sequence_extrapolation = 0;
        for (const auto& sequence : sequences | std::views::reverse | std::views::drop(1) ){
            current_sequence_extrapolation = sequence.extrapolate_backwards(current_sequence_extrapolation);
        }
        return current_sequence_extrapolation;
    }
};

class OasisReport{
    std::vector<OasisValueReport> reports{};
public:
    OasisReport(std::string_view record){
        for (const auto& line : split_lines(record)){
            reports.push_back(OasisValueReport(line));
        }
    }
    int sum_of_predictions(){
        return std::accumulate(
            reports.begin(), 
            reports.end(), 
            0, 
            [](int a, OasisValueReport b){return a + b.prediction();});
    }
    int sum_of_extrapolated_values(){
        return std::accumulate(
            reports.begin(), 
            reports.end(), 
            0, 
            [](int a, OasisValueReport b){return a + b.extrapolate_backwards();});
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
