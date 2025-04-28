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
            solution.write(f"""#include "../runner.hpp"
#include <string>
#include <optional>

#define DAY {day}
#define PART {part}
#define TEST true

std::optional<std::string> solve(std::string_view input){{
    return std::nullopt;
}}

int main(int argc, char *argv[]) {{
    if (argc > 1){{
        return runner(solve, input_loader(argv[1]));
    }}else{{
        return runner(solve, input_loader(DAY, PART, TEST));
    }}
}}
""")
