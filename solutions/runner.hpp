#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <sstream>
#include <filesystem>

using SolveFunctionType = std::optional<std::string>(*)(std::string_view);

int runner(SolveFunctionType, std::string_view);

std::string input_loader(int day, int part, bool test);
std::string input_loader(std::string_view file_path);
