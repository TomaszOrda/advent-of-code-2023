#include "runner.hpp"
#include <string>
#include <optional>

#define DAY 1
#define PART 1
#define TEST true

std::optional<std::string> solve(std::string_view input){
    return std::nullopt;
}

int main() {
    return runner(solve, input_loader(DAY, PART, TEST));
}
