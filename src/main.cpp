#include<iostream>

#include "./utils.hpp"
#include "./header_files/VM.hpp"

int main(int argc, char* argv[]) {
    std::string input_flag = "-i";
    std::string asm_file_path = "";

    std::string byte_count_flag = "-b";
    unsigned int byte_count = 256; 

    for (int i = 0; i < argc; i++) {
        if (argv[i] == input_flag) {
            asm_file_path = argv[i + 1];
        }

        if (argv[i] == byte_count_flag) {
            byte_count = std::stoi(argv[i + 1]);
        }
    }

    if (asm_file_path == "") {
        throw std::runtime_error("ERR: the path to the source file must be specified");
    }

    std::string asm_code = copy_asm_code(asm_file_path);
    VM vm(byte_count);
    vm.run(asm_code);
    return 0;
}
