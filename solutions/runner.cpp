#include "runner.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <sstream>
#include <filesystem>

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

int runner(SolveFunctionType solve, std::string_view input) {

    std::optional<std::string> output = solve(input);
    
    if (output){
        std::cout << output.value() << std::flush;
        return 0;
    }else{
        std::cout << "No output";
        return 1;
    }
}

std::string input_loader(std::string_view file_path){
    
    std::filesystem::path data_path {file_path};

    std::ifstream input_stream {data_path};
    if (!input_stream) {
        std::cerr << "Error opening " << data_path << "." << std::endl;
    }
    
    std::stringstream input_buffer;
    input_buffer << input_stream.rdbuf();

    return input_buffer.str();
}

std::string input_loader(int day, int part, bool test){
    
    std::string file_path = std::format("day_{}{}.txt",day, test ? "_test" : "");

    return input_loader(file_path);
}
