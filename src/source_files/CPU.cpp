#include "../header_files/CPU.hpp"

bool CPU::execute_instr(u8 opcode) {
    if (opcode == 0x00) {
        std::cout << "ERR: byte 0x00 detected\n";
        return false;
    }
    if (opcode == 0xFF) {
        std::cout << "halt_0xFF\n";
        return false;
    }
    auto it = instrs_set.find(opcode);
    if (it != instrs_set.end()) {
        it->second();
        return true;
    } else {
        std::cout 
        << "ERR: unknown opcode 0x" 
        << std::hex << std::setw(2) << std::setfill('0') 
        << static_cast<int>(opcode) << "\n";
        return false;
    }
}

void CPU::skip_instr(u8 opcode) {
    u16 prgc = mem.get_reg("prgc");

    if (opcode == 0x00) {
        mem.set_reg("prgc", prgc + 1);
        return;
    }

    if (opcode == 0x40) {
        u8 label = mem.fetch8();
        mem.add_label(label, prgc + 1);
    } else {
        u8 skip_size = instrs_size_set[opcode];
        mem.set_reg("prgc", prgc + skip_size);
    }
}

void CPU::mov_rr_0x10() {
    std::cout << "mov_rr_0x10\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u16 value = mem.get_reg(reg2);
    mem.set_reg(reg1, value);
}

void CPU::mov_rl_0x11() {
    std::cout << "mov_rl_0x11\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    mem.set_reg(reg, lit);
}

void CPU::add_rr_0x20() {
    std::cout << "add_rr_0x20\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    mem.set_reg(reg1, value1 + value2);
}

void CPU::add_rl_0x21() {
    std::cout << "add_rl_0x21\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value + lit);
}

void CPU::sub_rr_0x22() {
    std::cout << "sub_rr_0x22\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    mem.set_reg(reg1, value1 - value2);
}

void CPU::sub_rl_0x23() {
    std::cout << "sub_rl_0x23\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value - lit);
}

void CPU::mul_rr_0x24() {
    std::cout << "mul_rr_0x24\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    mem.set_reg(reg1, value1 * value2);
}

void CPU::mul_rl_0x25() {
    std::cout << "mul_rl_0x25\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value * lit);
}

void CPU::div_rr_0x26() {
    std::cout << "div_rr_0x26\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    mem.set_reg(reg1, value1 / value2);
}

void CPU::div_rl_0x27() {
    std::cout << "div_rl_0x27\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value / lit);
}

void CPU::inc_0x28() {
    std::cout << "inc_0x28\n";
    std::string reg = "r" + std::to_string(mem.fetch8());

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value + 1);
}

void CPU::dec_0x29() {
    std::cout << "dec_0x29\n";
    std::string reg = "r" + std::to_string(mem.fetch8());

    u16 value = mem.get_reg(reg);

    mem.set_reg(reg, value - 1);
}

void CPU::ifz_0x30() {
    std::cout << "ifz_0x30\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u8 label = mem.fetch8();

    if (mem.get_reg(reg) == 0) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifnz_0x31() {
    std::cout << "ifnz_0x31\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u8 label = mem.fetch8();

    if (mem.get_reg(reg) != 0) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifl_rr_0x32() {
    std::cout << "ifl_rr_0x32\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u8 label = mem.fetch8();

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    if (value1 < value2) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifl_rl_0x33() {
    std::cout << "ifl_rl_0x33\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u8 label = mem.fetch8();

    u16 value = mem.get_reg(reg);

    if (value < lit) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifm_rr_0x34() {
    std::cout << "ifm_rr_0x34\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u8 label = mem.fetch8();

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    if (value1 > value2) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifm_rl_0x35() {
    std::cout << "ifm_rl_0x35\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u8 label = mem.fetch8();

    u16 value = mem.get_reg(reg);

    if (value > lit) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ife_rr_0x36() {
    std::cout << "ife_rr_0x36\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u8 label = mem.fetch8();

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    if (value1 == value2) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ife_rl_0x37() {
    std::cout << "ife_rl_0x37\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u8 label = mem.fetch8();

    u16 value = mem.get_reg(reg);

    if (value == lit) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifne_rr_0x38() {
    std::cout << "ifne_rr_0x38\n";
    std::string reg1 = "r" + std::to_string(mem.fetch8());
    std::string reg2 = "r" + std::to_string(mem.fetch8());
    std::cout << "r1: " << reg1 << "\nr2: " << reg2 << "\n";

    u8 label = mem.fetch8();

    u16 value1 = mem.get_reg(reg1);
    u16 value2 = mem.get_reg(reg2);

    if (value1 != value2) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::ifne_rl_0x39() {
    std::cout << "ifne_rl_0x39\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 lit = mem.fetch16();
    std::cout << "r: " << reg << "\nl: " 
    << std::hex << std::setw(4) << std::setfill('0')
    << lit << "\n";

    u8 label = mem.fetch8();

    u16 value = mem.get_reg(reg);

    if (value != lit) {
        mem.set_reg("prgc", mem.get_label_prgc(label));
    }
}

void CPU::lbl_0x40() {
    std::cout << "lbl_0x40 - skip\n";
    u16 prgc = mem.get_reg("prgc");
    mem.set_reg("prgc", prgc + 1);
}

void CPU::jmp_0x41() {
    std::cout << "jmp_0x41\n";
    u8 label = mem.fetch8();
    mem.set_reg("prgc", mem.get_label_prgc(label));
}

void CPU::call_0x42() {
    std::cout << "call_0x42\n";
    u8 label = mem.fetch8();
    u16 addr_to = mem.get_label_prgc(label);
    u16 addr_from = mem.get_reg("prgc");

    mem.enter_frame();
    mem.push_to_stack(addr_from);

    mem.set_reg("prgc", addr_to);
}

void CPU::ret_0x43() {
    std::cout << "ret_0x43\n";
    u16 ret_addr = mem.pop_from_stack();
    mem.exit_frame();

    mem.set_reg("prgc", ret_addr);
}

void CPU::push_0x44() {
    std::cout << "push_0x44\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 value = mem.get_reg(reg);
    mem.push_to_stack(value);
}

void CPU::pop_0x45() {
    std::cout << "pop_0x45\n";
    std::string reg = "r" + std::to_string(mem.fetch8());
    u16 value = mem.pop_from_stack();
    mem.set_reg(reg, value);
}
