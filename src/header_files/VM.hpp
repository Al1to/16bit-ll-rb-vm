#pragma once

#include<iostream>
#include<string>
#include<stdexcept>

#include "../defines.hpp"
#include "CPU.hpp"
#include "MEM.hpp"

class VM {
    private:
        const std::vector<std::string> reg_names = {
            "prgc",
            "r1", "r2", "r3", "r4", 
            "r5", "r6", "r7", "r8",
            "sp", "fp"
        };
        CPU cpu;
        MEM mem;

        void load_to_mem(std::string& asm_code);
        bool rr_or_rl(std::string &asm_code, unsigned long long &cur);

    public:
        VM(unsigned int mem_size) 
        : mem(mem_size, reg_names), cpu(mem) {
            // std::cout << "VM constr\n";
        }

        void run(std::string& asm_code);
};
