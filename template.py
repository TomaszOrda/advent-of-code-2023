import os

with open("solutions/all_solutions.hpp", "w", encoding="UTF-8") as hpp_file:
    hpp_file.writelines(
        [
            "#pragma once\n\n",
            "#include <functional>\n",
            "#include <optional>\n",
            "#include <string>\n",
            "#include <array>\n\n"
        ]
    )
    for day in range(1, 26):
        data_path = f"data/day_{day}.txt"
        data_test_path = f"data/day_{day}_test.txt"
        if not os.path.isfile(data_path):
            open(data_path, "w", encoding="UTF-8").close()
        if not os.path.isfile(data_test_path):
            open(data_test_path, "w", encoding="UTF-8").close()
        for part in range(1, 3):
            solution_path = f"solutions/day_{day}_part_{part}.cpp"
            hpp_file.writelines([
                f"namespace day_{day}_part_{part}"+"{\n",
                "    std::optional<std::string> solve(std::string_view input);\n",
                "}\n"
            ])
            if os.path.isfile(solution_path):
                continue
            with open(solution_path, 'w', encoding="UTF-8") as solution:
                solution.writelines([
                    '#include "all_solutions.hpp"\n',
                    "#include <string>\n",
                    "#include <optional>\n",
                    "\n",
                    f"namespace day_{day}_part_{part}"+"{\n",
                    "    std::optional<std::string> solve(std::string_view input){\n",
                    "        return std::nullopt;\n",
                    "    }\n",
                    "}\n"
                ])
    hpp_file.writelines([
        "\n",
        "using SolveFunctionType = std::optional<std::string>(*)(std::string_view);\n",
        "\n",
        "static constexpr std::array<std::array<SolveFunctionType, 2>, 25> solutions ={{\n"
    ])
    for day in range(1, 26):
        hpp_file.write("    {"+f"day_{day}_part_1::solve, day_{day}_part_2::solve"+"},\n")
    hpp_file.writelines([
        "}};\n"
    ])
