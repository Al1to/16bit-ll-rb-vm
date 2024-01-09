#pragma once

#include<iostream>
#include<unordered_map>
#include<functional>

#include "../defines.hpp"
#include "./MEM.hpp"

class CPU {
    private: 
        MEM& mem;
        std::unordered_map<u8, std::function<void()>> instrs_set;
        std::unordered_map<u8, u8> instrs_size_set;

        void mov_rr_0x10();

        void mov_rl_0x11();

        void add_rr_0x20();

        void add_rl_0x21();

        void sub_rr_0x22();

        void sub_rl_0x23();

        void mul_rr_0x24();

        void mul_rl_0x25();

        void div_rr_0x26();

        void div_rl_0x27();

        void inc_0x28();

        void dec_0x29();

        void ifz_0x30();

        void ifnz_0x31();

        void ifl_rr_0x32();

        void ifl_rl_0x33();

        void ifm_rr_0x34();

        void ifm_rl_0x35();

        void ife_rr_0x36();

        void ife_rl_0x37();

        void ifne_rr_0x38();

        void ifne_rl_0x39();

        void lbl_0x40();

        void jmp_0x41();

        void call_0x42();

        void ret_0x43();

        void push_0x44();

        void pop_0x45();

    public:
        CPU(MEM& memory) : mem(memory) {
            // std::cout << "CPU constr\n";
            instrs_set = {
                { 0x10, [this]() {  mov_rr_0x10(); } },
                { 0x11, [this]() {  mov_rl_0x11(); } },

                { 0x20, [this]() {  add_rr_0x20(); } },
                { 0x21, [this]() {  add_rl_0x21(); } },

                { 0x22, [this]() {  sub_rr_0x22(); } },
                { 0x23, [this]() {  sub_rl_0x23(); } },

                { 0x24, [this]() {  mul_rr_0x24(); } },
                { 0x25, [this]() {  mul_rl_0x25(); } },

                { 0x26, [this]() {  div_rr_0x26(); } },
                { 0x27, [this]() {  div_rl_0x27(); } },

                { 0x28, [this]() {     inc_0x28(); } },

                { 0x29, [this]() {     dec_0x29(); } },

                { 0x30, [this]() {     ifz_0x30(); } },
                { 0x31, [this]() {    ifnz_0x31(); } },

                { 0x32, [this]() {  ifl_rr_0x32(); } },
                { 0x33, [this]() {  ifl_rl_0x33(); } },

                { 0x34, [this]() {  ifm_rr_0x34(); } },
                { 0x35, [this]() {  ifm_rl_0x35(); } },

                { 0x36, [this]() {  ife_rr_0x36(); } },
                { 0x37, [this]() {  ife_rl_0x37(); } },

                { 0x38, [this]() { ifne_rr_0x38(); } },
                { 0x39, [this]() { ifne_rl_0x39(); } },

                { 0x40, [this]() {     lbl_0x40(); } },

                { 0x41, [this]() {     jmp_0x41(); } },

                { 0x42, [this]() {    call_0x42(); } },
                { 0x43, [this]() {     ret_0x43(); } },

                { 0x44, [this]() {    push_0x44(); } },
                { 0x45, [this]() {     pop_0x45(); } },
            };

            instrs_size_set = {
                {0x10, 2}, // mov r r
                {0x11, 3}, // mov r l

                {0x20, 2}, // add r r
                {0x21, 3}, // add r l
                {0x22, 2}, // sub r r
                {0x23, 3}, // sub r l
                {0x24, 2}, // mul r r
                {0x25, 3}, // mul r l
                {0x26, 2}, // div r r
                {0x27, 3}, // div r l
                {0x28, 1}, // inc r
                {0x29, 1}, // dec r

                {0x30, 2}, // ifz r lbl
                {0x31, 2}, // ifnz r lbl
                {0x32, 3}, // ifl r r lbl
                {0x33, 4}, // ifl r l lbl
                {0x34, 3}, // ifm r r lbl
                {0x35, 4}, // ifm r l lbl
                {0x36, 3}, // ife r r lbl
                {0x37, 4}, // ife r l lbl
                {0x38, 3}, // ifne r r lbl
                {0x39, 4}, // ifne r l lbl

                // 0x40 lbl - skip
                {0x41, 1}, // jmp lbl
                {0x42, 1}, // call lbl
                {0x43, 0}, // ret
                {0x44, 1}, // push r
                {0x45, 1}, // pop r
            };
        }

        bool execute_instr(u8 opcode);

        void skip_instr(u8 opcode);
};
