#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <sstream>
#include <filesystem>

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

// int runner(SolveFunctionType, std::string_view);

// std::string input_loader(int day, int part, bool test);
// std::string input_loader(std::string_view file_path);

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

class Solution{
public:
    Solution(int day, int part, SolveFunctionType solve);
    
    int test_run();
    int test_run(std::string_view input);
    int run();
    int run(std::string_view file_path);
private:
    int m_day;
    int m_part;
    SolveFunctionType m_solve;
    int runner(SolveFunctionType solve, std::string_view input);
    std::string input_loader(std::string_view file_path);
    std::string input_loader(int day, int part, bool test);
};
