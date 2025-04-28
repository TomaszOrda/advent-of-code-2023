#include "../runner.hpp"
#include <string>
#include <optional>

#define DAY 1
#define PART 2
#define TEST true

std::optional<std::string> solve(std::string_view input){
    return std::nullopt;
}

int main(int argc, char *argv[]) {
    if (argc > 1){
        return runner(solve, input_loader(argv[1]));
    }else{
        return runner(solve, input_loader(DAY, PART, TEST));
    }
}
