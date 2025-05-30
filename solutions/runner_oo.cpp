#include "runner_oo.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <sstream>
#include <filesystem>

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

Solution::Solution(int day, int part, SolveFunctionType solve)
    : m_day { day }
    , m_part { part }
    , m_solve {solve}
{
}
    
int Solution::test_run(){
    return this->runner(m_solve, this->input_loader(m_day, m_part, true));
}
int Solution::test_run(std::string_view input){
    return this->runner(m_solve, input);
}
int Solution::run(){
    return this->runner(m_solve, this->input_loader(m_day, m_part, false));
}
int Solution::run(std::string_view file_path){
    return this->runner(m_solve, this->input_loader(file_path));
}
int Solution::runner(SolveFunctionType solve, std::string_view input) {

    std::optional<std::string> output = solve(input);
    
    if (output){
        std::cout << output.value() << std::flush;
        return 0;
    }else{
        std::cout << "No output";
        return 1;
    }
}

std::string Solution::input_loader(std::string_view file_path){
    
    std::filesystem::path data_path {file_path};

    std::ifstream input_stream {data_path};
    if (!input_stream) {
        std::cerr << "Error opening " << data_path << "." << std::endl;
    }
    
    std::stringstream input_buffer;
    input_buffer << input_stream.rdbuf();

    return input_buffer.str();
}

std::string Solution::input_loader(int day, int part, bool test){
    
    std::string file_path = std::format("day_{}{}.txt",day, test ? "_test" : "");

    return input_loader(file_path);
}

