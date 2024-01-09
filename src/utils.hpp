#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

std::string copy_asm_code(std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("ERR: could not open file \"" + path + "\"");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (file.fail()) {
        throw std::runtime_error("ERR: failed to read from file \"" + path + "\"");
    }
    return buffer.str();
}
