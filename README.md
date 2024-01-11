# 16-bit Low-Level Register-based Virtual Machine
###### This VM is one of my first attempts. It is only needed for training for more serious projects.
This VM has its own assembly language. It is translated into bytecode that is already being executed.

Here is a list of all instructions implemented in assembly language (their opcodes are indicated through a dash):
 - `mov` – 0x10 or 0x11 
 - `add` – 0x20 or 0x21
 - `sub` – 0x22 or 0x23
 - `mul` – 0x24 or 0x25
 - `div` – 0x26 or 0x27
 - `inc` – 0x28
 - `dec` – 0x29
 - `ifz` – 0x30 *(if zero)*
 - `ifnz` – 0x31 *(if not zero)*
 - `ifl` – 0x32 or 0x33 *(if less)*
 - `ifm` – 0x34 or 0x35 *(if more)*
 - `ife` – 0x36 or 0x37 *(if equal)*
 - `ifne` – 0x38 or 0x39 *(if not equal)*
 - `jmp` – 0x41
 - `call` – 0x42
 - `ret` – 0x43
 - `push` – 0x44
 - `pop` – 0x45
 - `halt` – 0xFF

Many instructions have two opcodes, since the opcodes for instructions like `instr reg, reg` and `instr reg, lit` are different.
In the first case, opcodes for **rr** were indicated, and in the second, for **rl**

There are a total of 11 registers in this virtual machine (they are all 16 bits):
- `prgc` *(The name of this register stands for program counter. It is necessary for the operation of the processor. It cannot be accessed from assembly language)*
- from `r1` to `r8` inclusive *(These registers can be accessed from assembly language. You can store numbers in them, work with them somehow, etc. Everything is like in standard assembly languages)*
- `sp` and `fp` *(They are needed for the stack and related instructions to work. Their starting values ​​are 0xFFFF. They cannot be accessed in my assembly language)*

Registers `r1` to `r8` also have their own opcodes 0x01 to 0x08. <br>
Labels also have their own opcodes. (Each new label is saved with its own number, which is the label's opcode). <br>
There is also an auxiliary opcode 0x40. It is needed to indicate to the preprocessor that a label is being created at this location. The processor skips this opcode. <br>

It’s also worth saying that I did not implement the _start, _main or etc. labels.

### Examples of VM operation:

#### 1 Example
Let's say we have this code in assembly language:
```asm
mov r1, 5
mov r2, r1
halt
```

You can run this code with the command (of course, provided that our code is written in the example1.asm file and the vm executable file is nearby):
```bash
./vm -i ./example1.asm -b 64
```
[Find out more about the flags when starting a virtual machine here.](#flags)

And we will see this output:
```bash
dbg_log_mem
11 01 00 05 10 02 01 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

mov_rl_0x11
r: r1
l: 0005
mov_rr_0x10
r1: r2
r2: r1
halt_0xFF

VM stopped

dbg_log_stack

dbg_log_regs_mem
prgc r1   r2   r3   r4   r5   r6   r7   r8   sp   fp
0008 0005 0005 0000 0000 0000 0000 0000 0000 ffff ffff
```

At the beginning, the entire memory is output after the code is loaded into it. <br>
Next comes the output from the processor, in what order the instructions were executed. This is quite convenient for debugging. <br>
Next comes the output of all values from the stack. *(In this example the stack is empty)* <br>
Then the values of all registers are output.

Let's look at memory:
```bash
11 01 00 05 10 02 01 ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

 - byte `11` is the mov_reg_lit instruction
 - byte `01` is register r1
 - bytes `00` and `05` is the literal 5
 - byte `10` is the mov_reg_reg instruction
 - byte `02` is register r2
 - byte `01` is register r1
 - byte `ff` is the halt instruction

Next, as expected, two mov instructions are executed first and then halt. <br>
At the end, registers r1 and r2 contain fives.

#### 2 Example

Let's take a more complex code in assembly language:
```asm
mov r1, 3
mov r2, 3

mul r1, r2

ifm r1, r2 _label_one
halt

_label_one:
    call _square
    ife r1, r2 _label_two
    halt

_label_two:
    inc r4
    halt

_square:
    push r3
    mov r3, r2
    mul r2, r3
    pop r3

    ret
```

Run:
```bash
./vm -i ./example2.asm
```

Output:
```bash
dbg_log_mem
11 01 00 03 11 02 00 03 24 01 02 34 01 02 00 ff 40 00 42 01 36 01 02 02 ff 40 02 28 04 ff 40 01 
44 03 10 03 02 24 02 03 45 03 43 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

mov_rl_0x11
r: r1
l: 0003
mov_rl_0x11
r: r2
l: 0003
mul_rr_0x24
r1: r1
r2: r2
ifm_rr_0x34
r1: r1
r2: r2
call_0x42
push_0x44
mov_rr_0x10
r1: r3
r2: r2
mul_rr_0x24
r1: r2
r2: r3
pop_0x45
ret_0x43
ife_rr_0x36
r1: r1
r2: r2
inc_0x28
halt_0xFF

VM stopped

dbg_log_stack

dbg_log_regs_mem
prgc r1   r2   r3   r4   r5   r6   r7   r8   sp   fp
001e 0009 0009 0000 0001 0000 0000 0000 0000 ffff ffff
```

If you want, you can calculate it in your head. Everything works correctly.

<a id="flags"></a>
### Flags when starting a VM

 - `-i` – This flag is required at startup. After it the path to the file with the code in assembly language is indicated.
 - `-b` – This flag is optional at startup. It is followed by a non-negative number. Using it you can set the number of bytes in memory. According to the standard, the number of bytes in memory is 256.
