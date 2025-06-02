#include "../runner.hpp"
#include "../utils.hpp"
#include <string>
#include <optional>
#include <vector>

int to_number(char c){
    return c - '0';
}
bool is_number(char c){
    return '0'<= c && c <='9';
}
bool is_symbol(char c){
    return c!='.' && !is_number(c);
}
bool is_part_number(std::pair<int, int> number_position, const Grid<char>& schematic_grid){
    int x = number_position.first;
    int y = number_position.second;
    for (int offset_x{-1}; offset_x<=1; offset_x++){
        for (int offset_y{-1}; offset_y<=1; offset_y++){
            if (is_symbol(schematic_grid.at(x+offset_x, y+offset_y).value_or('.'))){
                return true;
            }
        }
    }
    if (is_number(schematic_grid.at(x-1, y).value_or('.'))){
        return is_part_number(std::pair<int, int>(x-1 ,y), schematic_grid);
    }
    return false;
}
int number_pos_to_number(std::pair<int, int> number_position, const Grid<char>& schematic_grid){
    int x = number_position.first;
    int y = number_position.second;
    int value = to_number(schematic_grid.at(x,y).value());
    if (is_number(schematic_grid.at(x-1, y).value_or('.'))){
        return value + 10*number_pos_to_number(std::pair<int, int>(x-1 ,y), schematic_grid);
    }
    return value;
}

std::optional<std::string> solve(std::string_view input){
    Grid<char> schematic_grid(input);
    std::vector<std::pair<int, int>> number_positions{};
    for (int y{0}; y<schematic_grid.get_height(); y++){
        for (int x{0}; x<schematic_grid.get_width(); x++){
            char character = schematic_grid.at(x,y).value_or('.');
            char successor = schematic_grid.at(x+1,y).value_or('.');
            if (is_number(character) && !is_number(successor)){
                number_positions.push_back(std::pair<int, int>(x,y));
            }
        } 
    }
    int sum_of_part_numbers = 0;
    for (const std::pair<int, int>& pos : number_positions){
        if (is_part_number(pos, schematic_grid)){
            sum_of_part_numbers += number_pos_to_number(pos, schematic_grid);
        }
    }
    return std::to_string(sum_of_part_numbers);
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    Solution solution = Solution(3, 1, solve);
    if (argc > 1){
        return solution.test_run();
    }else{
        return solution.run();
    }
}
