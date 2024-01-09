#pragma once

#include<iostream>
#include<iomanip>
#include<unordered_map>
#include<vector>
#include<string>
#include<stdexcept>

#include "../defines.hpp"

class MEM {
    private:
        std::vector<u8> mem;
        std::vector<u8> stack;
        std::vector<u8> regs_mem;
        std::unordered_map<std::string, size_t> regs;
        std::unordered_map<u8, u16> labels;

    public:
        MEM(
            unsigned int mem_size_in_bytes, 
            std::vector<std::string> reg_names
        ) {
            // std::cout << "MEM constr\n";

            mem.resize(mem_size_in_bytes, 0);

            regs_mem.resize(reg_names.size() * 2, 0);
            for (size_t i = 0; i < reg_names.size(); ++i) {
                regs[reg_names[i]] = i * 2;
            }
        }

        u8 fetch8();
        u16 fetch16();

        void write(size_t index, u8 value);
        u8 read(size_t index) const;

        int mem_len();

        void set_stack_ptr();

        void push_to_stack(u16 value);
        u16 pop_from_stack();

        void enter_frame();
        void exit_frame();

        void set_reg(const std::string& name, u16 value);
        u16 get_reg(const std::string& name) const;

        void add_label(u8 number, u16 prgc);
        u16 get_label_prgc(u8 number);

        void dbg_log_mem();
        void dbg_log_stack();
        void dbg_log_regs_mem();
};
