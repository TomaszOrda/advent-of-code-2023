#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <ranges>
#include <numeric>

#define HASH_MULTIPLIER 17

class LensLabel{
    std::string data;
public:
    LensLabel(std::string_view text) : data{text}
    {};
    std::string_view get_data() const{
        return data;
    }
};

template<>
struct std::hash<LensLabel>{
    std::size_t operator()(const LensLabel& init_step){
        size_t current_hash = 0;
        std::string_view data = init_step.get_data();
        for (size_t i{}; i< data.length(); i++){
            current_hash += static_cast<size_t>(data[i]);
            current_hash *= HASH_MULTIPLIER;
            current_hash %= 256;
        }
        return current_hash;
    }
};

std::optional<std::string> solve(std::string_view input){
    auto init_steps = split(input, ',') | std::views::transform([](auto x){return std::hash<LensLabel>{}(LensLabel(x));});
    return std::to_string(std::accumulate(init_steps.begin(), init_steps.end(), 0ULL));
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(15, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
