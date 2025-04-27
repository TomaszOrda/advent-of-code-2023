#pragma once

#include <functional>
#include <optional>
#include <string>
#include <array>

namespace day_1_part_1{
    std::optional<std::string> solve(std::string_view input);
}
namespace day_1_part_2{
    std::optional<std::string> solve(std::string_view input);
}

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

static constexpr std::array<std::array<SolveFunctionType, 2>, 1> solutions ={{
    {day_1_part_1::solve, day_1_part_2::solve}
}};