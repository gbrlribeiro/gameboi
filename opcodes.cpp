#include "cpu.hpp"
#include "common.hpp"

//LD instructions

void op_ld(uint8_t &res, uint8_t &reg){
    res = reg;
}

void op_ld(uint8_t &res){
    res = CPU.read(CPU.pc);
}

void op_ld(uint8_t &res, uint16_t addr){
    res = CPU.read(addr);
}

void op_ld(uint16_t addr, uint8_t reg){
    CPU.write(addr, reg);
}

void op_ld(uint16_t addr){
    CPU.write(addr, CPU.read(CPU.pc));
}


//LDH instructions
void op_ldh_acc(){
    uint16_t addr = (0xFF00 | CPU.BC);
    CPU.acc = CPU.read(addr);
}

void op_ldh_c(){
    uint16_t addr = (0xFF00 | CPU.BC);
    CPU.write(addr, CPU.acc);
}

void op_ldh_acc_n(){    
    uint16_t addr = (0xFF00 | CPU.read(CPU.pc));
    CPU.acc = CPU.read(addr);
}

void op_ldh_n_acc(){
    uint16_t addr = (0xFF00 | CPU.read(CPU.pc));
    CPU.write(addr, CPU.acc);
}

//LD with decrement
void op_lddecA(){
    CPU.acc = CPU.read(CPU.HL);
    CPU.HL--;
}

void op_lddecHL(){
    CPU.write(CPU.HL, CPU.acc);
    CPU.HL--;
}

//LD with increment

void op_ldincA(){
    CPU.acc = CPU.read(CPU.HL);
    CPU.HL++;
}

void op_ldincHL(){
    CPU.write(CPU.HL, CPU.acc);
    CPU.HL--;
}

//16-bit LD
void op_16b_ld(uint16_t &reg){
    uint16_t val = static_cast<uint16_t>(CPU.read(CPU.pc));
    CPU.pc++;
    reg = (0xFF00 | ((static_cast<uint16_t>(CPU.read(CPU.pc)))<<8) | val);
}

void op_16b_sp(){
    uint16_t addr = static_cast<uint16_t>(CPU.read(CPU.pc));
    CPU.pc++;
    addr = ((static_cast<uint16_t>(CPU.read(CPU.pc))) << 8) | addr;
    uint8_t _sp = static_cast<uint8_t>(0x00FF & CPU.sp);
    CPU.write(addr, _sp);
    addr++;
    _sp = static_cast<uint8_t>(0xFF00 & CPU.sp);
    CPU.write(addr, _sp);
}

void op_16b_hl(){
    CPU.sp = CPU.HL;
}

//PUSH instructions
void op_16_push(uint16_t reg){
    CPU.sp--;
    uint8_t val = static_cast<uint8_t>((0xFF00 & reg) >> 8);
    CPU.write_stack(CPU.sp, val);
    CPU.sp--;
    val = static_cast<uint8_t>(0x00FF & reg);
    CPU.write_stacl(CPU.sp, val);
    CPU.sp--;
}

void op_16_pop(uint16_t &reg){
    CPU.sp++;
    reg = (0x00FF & CPU.read_stack(CPU.sp));
    CPU.sp++;
    reg |= CPU.read_stack(CPU.sp);
}

//JMP

void op_jmp_abs(){
    uint16_t addr = static_cast<uint16_t>(CPU.read(CPU.pc)) & 0x00FF;
    CPU.pc++;
    addr = ((static_cast<uint16_t>(CPU.read(CPU.pc))) & 0xFF00) | addr;
    CPU.pc = addr; 
}


void op_jmp_hl(){
    CPU.pc = CPU.HL;
}

void op_jmp_cond(condition Condition){
    if(check_condition(Condition)){
        op_jmp_abs();
        CPU.branch_taken = 1;
    }
    else{
        CPU.branch_taken = 0;
        CPU.pc = CPU.pc + 2;  
    }
}

void op_jmp_rel(){
    int8_t offset = CPU.read_signed(CPU.pc);
    CPU.pc++;
    CPU.pc = static_cast<uint16_t>(CPU.pc + offset);
}

void op_jmp_cond(condition Condition){
    int8_t offset = CPU.read_signed(CPU.pc);
    CPU.pc++;
    if(check_condition(Condition)){
        CPU.branch_taken = 1;
        CPU.pc = static_cast<uint16_t>(CPU.pc + offset);
    } 
    else{
        CPU.branch_taken = 0;
        CPU.pc++;
    }
}

void op_call(){
    uint16_t addr = (static_cast<uint16_t>(CPU.read(CPU.pc))) & 0x00FF;
    CPU.pc++;
    addr = ((static_cast<uint16_t>(CPU.read(CPU.pc))) & 0xFF00) | addr;
    CPU.pc++;
    CPU.sp--;
    uint8_t val = static_cast<uint8_t>((0xFF00 & CPU.pc) >> 8);
    CPU.write_stack(CPU.sp, val);
    CPU.pc++;
    CPU.sp--;
    val = static_cast<uint8_t>(0x00FF & CPU.pc);
    CPU.write_stack(CPU.sp, val);
    CPU.pc = addr;
}

void op_call_nn(){

}