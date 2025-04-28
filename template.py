import os

for day in range(1, 26):
    folder_path = f"solutions/day_{day}"
    if not os.path.exists(folder_path):
        os.mkdir(folder_path)

    data_path = folder_path + f"/day_{day}.txt"
    data_test_path = folder_path + f"/day_{day}_test.txt"

    if not os.path.isfile(data_path):
        open(data_path, "w", encoding="UTF-8").close()
    if not os.path.isfile(data_test_path):
        open(data_test_path, "w", encoding="UTF-8").close()

    for part in range(1, 3):
        solution_path = folder_path + f"/day_{day}_part_{part}.cpp"
        if os.path.isfile(solution_path):
            continue

        with open(solution_path, 'w', encoding="UTF-8") as solution:
            solution.writelines([
                '#include "../runner.hpp"\n'
                "#include <string>\n",
                "#include <optional>\n\n",
                f'#define DAY {day}\n'
                f'#define PART {part}\n'
                '#define TEST true\n\n'
                "std::optional<std::string> solve(std::string_view input){\n",
                "    return std::nullopt;\n",
                "}\n\n",
                'int main(int argc, char *argv[]) {\n'
                '    if (argc > 1){\n'
                '        return runner(solve, input_loader(argv[1]));\n'
                '    }else{\n'
                '        return runner(solve, input_loader(DAY, PART, TEST));\n'
                '    }\n'
                '}\n'
            ])
