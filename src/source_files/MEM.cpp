#include "../defines.hpp"
#include"../header_files/MEM.hpp"

u8 MEM::fetch8() {
    size_t prgc = get_reg("prgc");
    u8 instr = mem[prgc];
    set_reg("prgc", prgc + 1);
    return instr;
}

u16 MEM::fetch16() {
    size_t prgc = get_reg("prgc");
    u16 instr = mem[prgc + 1] | (mem[prgc] << 8);
    set_reg("prgc", prgc + 2);
    return instr;
}

void MEM::write(size_t index, u8 value) {
    mem[index] = value;
}

u8 MEM::read(size_t index) const {
    return mem[index];
}

int MEM::mem_len() {
    return mem.size();
}

void MEM::set_stack_ptr() {
    set_reg("sp", 0xFFFF);
    set_reg("fp", 0xFFFF);
}

void MEM::push_to_stack(u16 value) {
    stack.push_back(value >> 8);
    stack.push_back(value & 0xFF);

    u16 sp = get_reg("sp");
    set_reg("sp", sp + 2);
}

u16 MEM::pop_from_stack() {
    u16 sp = get_reg("sp");
    if (sp == 0xFFFF) {
        throw std::out_of_range("ERR: stack is empty");
    }

    u16 value = (stack[sp - 1] << 8) | stack[sp];
    stack.pop_back();
    stack.pop_back();

    set_reg("sp", sp - 2);

    return value;
}

void MEM::enter_frame() {
    u16 fp = get_reg("fp");
    push_to_stack(fp);

    u16 sp = get_reg("sp");

    set_reg("fp", sp);
}

void MEM::exit_frame() {
    u16 fp = get_reg("fp");

    if (fp == 0xFFFF) {
        throw std::out_of_range("ERR: no frame to exit");
    }

    set_reg("sp", fp);
    set_reg("fp", pop_from_stack());
}

void MEM::set_reg(const std::string& name, u16 value) {
    if (value > 0xFFFF) {
        throw std::runtime_error("ERR: the number exceeds the limits of 16 bits");
    }
    auto it = regs.find(name);
    if (it == regs.end()) {
        throw std::runtime_error("ERR: reg " + name + " not found");
    }
    size_t i = it->second;
    regs_mem[i + 1] = value & 0xFF;
    regs_mem[i] = value >> 8;
}

u16 MEM::get_reg(const std::string& name) const {
    auto it = regs.find(name);
    if (it == regs.end()) {
        throw std::runtime_error("ERR: reg " + name + " not found");
    }
    size_t i = it->second;
    return regs_mem[i + 1] | (regs_mem[i] << 8);
}

void MEM::add_label(u8 number, u16 prgc) {
    labels[number] = prgc;
}

u16 MEM::get_label_prgc(u8 number) {
    if (labels.find(number) == labels.end()) {
        throw std::runtime_error("ERR: label number " + std::to_string(number) + " not found");
    }
    return labels[number];
}

void MEM::dbg_log_mem() {
    std::cout << "dbg_log_mem\n";
    int count = 0;
    for (u8 i : mem) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i) << " ";
        count++;
        if (count == 32) {
            std::cout << "\n";
            count = 0;
        }
    }
    std::cout << "\n";
}

void MEM::dbg_log_stack() {
    std::cout << "dbg_log_stack\n";
    int count = 0;
    for (u8 i : stack) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        count++;
        if (count == 2) {
            std::cout << "\n";
            count = 0;
        }
    }
    std::cout << "\n";
}

void MEM::dbg_log_regs_mem() {
    std::cout << "dbg_log_regs_mem\n";
    int count = 0;
    for (u8 i : regs_mem) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        count++;
        if (count == 2) {
            std::cout << " ";
            count = 0;
        }
    }
    std::cout << "\n";
}
