#include "../defines.hpp"
#include "../header_files/VM.hpp"

void VM::run(std::string& asm_code) {
    load_to_mem(asm_code);

    while (mem.get_reg("prgc") < mem.mem_len()) {
        cpu.skip_instr(mem.fetch8());
    }

    mem.set_reg("prgc", 0x0000);
    mem.set_stack_ptr();

    mem.dbg_log_mem();

    while (mem.get_reg("prgc") < mem.mem_len()) {
        bool is_run = cpu.execute_instr(mem.fetch8());
        if (!is_run) {
            std::cout << "\nVM stopped\n\n";
            break;
        }
    }

    mem.dbg_log_stack();
    mem.dbg_log_regs_mem();
}

void VM::load_to_mem(std::string& asm_code) {
    // std::cout << "load_to_mem\n";

    std::unordered_map<std::string, u8> label_numbers;
    u8 labels_counter = 0;

    unsigned long long cur = 0;
    unsigned int cur_byte = 0;

    // std::cout << "code len: " << asm_code.length() << "\n";

    while (cur < asm_code.length()) {
        // std::cout << "cur: " << cur << ", " << std::to_string(asm_code[cur]) << "\n";

        while (isspace(asm_code[cur]) || asm_code[cur] == ',') {
            ++cur;
        }

        if (asm_code[cur] == 'r' && isdigit(asm_code[cur + 1])) {
            ++cur;
            char reg_num = asm_code[cur] - '0';
            mem.write(cur_byte, reg_num);
            ++cur_byte;
            ++cur;

        } else if (asm_code[cur] == '_') {
            std::string lexeme = std::string() + asm_code[cur];
            ++cur;

            while (isalpha(asm_code[cur]) || asm_code[cur] == '_') {
                lexeme += std::string() + asm_code[cur];
                ++cur;
            }

            auto it = label_numbers.find(lexeme);

            if (it == label_numbers.end()) {
                label_numbers.insert({lexeme, labels_counter});
                ++labels_counter;

                if (asm_code[cur] == ':') {
                    mem.write(cur_byte, 0x40);
                    ++cur_byte;
                    ++cur;

                    mem.write(cur_byte, labels_counter - 1);
                    ++cur_byte;
                } else {
                    mem.write(cur_byte, labels_counter - 1);
                    ++cur_byte;
                }
            } else {
                if (asm_code[cur] == ':') {
                    mem.write(cur_byte, 0x40);
                    ++cur_byte;
                    ++cur;

                    mem.write(cur_byte, it->second);
                    ++cur_byte;
                } else {
                    mem.write(cur_byte, it->second);
                    ++cur_byte;
                }
            }

        } else if (isalpha(asm_code[cur])) {
            std::string lexeme = std::string() + asm_code[cur];
            ++cur;

            while (isalpha(asm_code[cur])) {
                lexeme += std::string() + asm_code[cur];
                ++cur;
            }

            if (lexeme == "halt") {
                // std::cout << "halt\n";
                mem.write(cur_byte, 0xFF);
                ++cur_byte;
            }

            else if (lexeme == "mov") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "movrr\n";
                    mem.write(cur_byte, 0x10);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "movrl\n";
                    mem.write(cur_byte, 0x11);
                    ++cur_byte;
                }
            }

            else if (lexeme == "add") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "addrr\n";
                    mem.write(cur_byte, 0x20);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "addrl\n";
                    mem.write(cur_byte, 0x21);
                    ++cur_byte;
                }
            }

            else if (lexeme == "sub") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "subrr\n";
                    mem.write(cur_byte, 0x22);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "subrl\n";
                    mem.write(cur_byte, 0x23);
                    ++cur_byte;
                }
            }

            else if (lexeme == "mul") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "mulrr\n";
                    mem.write(cur_byte, 0x24);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "mulrl\n";
                    mem.write(cur_byte, 0x25);
                    ++cur_byte;
                }
            }

            else if (lexeme == "div") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "divrr\n";
                    mem.write(cur_byte, 0x26);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "divrl\n";
                    mem.write(cur_byte, 0x27);
                    ++cur_byte;
                }
            }

            else if (lexeme == "inc") {
                // std::cout << "inc\n";
                mem.write(cur_byte, 0x28);
                ++cur_byte;
            }

            else if (lexeme == "dec") {
                // std::cout << "dec\n";
                mem.write(cur_byte, 0x29);
                ++cur_byte;
            }

            else if (lexeme == "ifz") {
                // std::cout << "ifz\n";
                mem.write(cur_byte, 0x30);
                ++cur_byte;
            }

            else if (lexeme == "ifnz") {
                // std::cout << "ifnz\n";
                mem.write(cur_byte, 0x31);
                ++cur_byte;
            }

            else if (lexeme == "ifl") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "iflrr\n";
                    mem.write(cur_byte, 0x32);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "iflrl\n";
                    mem.write(cur_byte, 0x33);
                    ++cur_byte;
                }
            }

            else if (lexeme == "ifm") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "ifmrr\n";
                    mem.write(cur_byte, 0x34);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "ifmrl\n";
                    mem.write(cur_byte, 0x35);
                    ++cur_byte;
                }
            }

            else if (lexeme == "ife") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "iferr\n";
                    mem.write(cur_byte, 0x36);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "iferl\n";
                    mem.write(cur_byte, 0x37);
                    ++cur_byte;
                }
            }

            else if (lexeme == "ifne") {
                if (rr_or_rl(asm_code, cur)) {
                    // true - reg reg
                    // std::cout << "ifnerr\n";
                    mem.write(cur_byte, 0x38);
                    ++cur_byte;
                } else {
                    // false - reg lit
                    // std::cout << "ifnerl\n";
                    mem.write(cur_byte, 0x39);
                    ++cur_byte;
                }
            }

            else if (lexeme == "jmp") {
                // std::cout << "jmp\n";
                mem.write(cur_byte, 0x41);
                ++cur_byte;
            }

            else if (lexeme == "call") {
                // std::cout << "call\n";
                mem.write(cur_byte, 0x42);
                ++cur_byte;
            }

            else if (lexeme == "ret") {
                // std::cout << "ret\n";
                mem.write(cur_byte, 0x43);
                ++cur_byte;
            }

            else if (lexeme == "push") {
                // std::cout << "push\n";
                mem.write(cur_byte, 0x44);
                ++cur_byte;
            }

            else if (lexeme == "pop") {
                // std::cout << "pop\n";
                mem.write(cur_byte, 0x45);
                ++cur_byte;
            }

        } else if (isdigit(asm_code[cur])) {
            std::string lexeme = std::string() + asm_code[cur];
            ++cur;

            while (isdigit(asm_code[cur])) {
                lexeme += std::string() + asm_code[cur];
                ++cur;
            }

            unsigned int num = std::stoi(lexeme);

            if (num > 0xFFFF) {
                throw std::runtime_error("ERR: the number exceeds the limits of 16 bits");
            }

            mem.write(cur_byte, num >> 8);
            ++cur_byte;
            mem.write(cur_byte, num & 0xFF);
            ++cur_byte;
        }
    }
}

bool VM::rr_or_rl(std::string &asm_code, unsigned long long &cur) {
    // std::cout << "rr_or_rl\n";
    unsigned long long save_cur = cur;

    while (isspace(asm_code[cur]) || asm_code[cur] == ',') {
        ++cur;
    }

    if (asm_code[cur] == 'r') {
        cur += 2;
    }

    while (isspace(asm_code[cur]) || asm_code[cur] == ',') {
        ++cur;
    }

    if (asm_code[cur] == 'r') {
        cur = save_cur;
        return true;
    } else if (isdigit(asm_code[cur])) {
        cur = save_cur;
        return false;
    }
    
    throw std::runtime_error("ERR: syntax error, expected `instr reg, reg` or `instr reg lit`");
}
