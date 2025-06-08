#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>

std::optional<std::string> solve(std::string_view input){
    return std::nullopt;
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(10, 2, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
