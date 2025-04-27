#include "solutions/all_solutions.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <sstream>
#include <filesystem>

int main(int argc, char* argv[]) {
    const int day = std::stoi(argv[1]);
    const int part = std::stoi(argv[2]);
    const bool test = argc>=4 && (argv[3][0]=='t');
 
    std::filesystem::path data_path {std::format("data/day_{}{}.txt",day, test ? "_test" : "")};

    std::ifstream input_stream {data_path};
    if (!input_stream) {
        std::cerr << "Error opening " << data_path << "." << std::endl;
    }
    
    std::stringstream input_buffer;
    input_buffer << input_stream.rdbuf();
    std::string input = input_buffer.str();

    std::optional<std::string> output = solutions[day-1][part-1](input);
    
    if (output){
        std::cout << output.value() << std::flush;
        return 0;
    }else{
        std::cout << "No output";
        return 1;
    }
}