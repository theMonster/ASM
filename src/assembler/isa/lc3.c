//
//  lc3.c
//  ARM
//
//  Created by Caleb on 7/1/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "lc3.h"

/* define these */
const uint16_t isa_bit_count = 16;
const uint16_t isa_opcode_size = 4;
const uint16_t isa_register_size = 3;
/// system register indecies
const isa_register_t IMMUTABLE_VALUE_REGISTER_ADDRESS = 0;
const isa_register_t PROGRAM_COUNTER_REGISTER_ADDRESS = 1;

// all grammars need to add up to the isa_bit_count
const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS] = {
    "ADD DR SR 000 SR",
    "ADD DR SR IMM6",
    "AND DR SR 000 SR",
    "AND DR SR IMM6",
    "MUL DR SR 000 SR",
    "MUL DR SR 0 IMM5",
    "ST DR 0000000 SR",
    "ST DR 00 IMM8",
    "JMP IMM12"
};

// all instructions should take "operation registers"
// each operation register is 1-1 for each register described in the assembly.
void isa_add(struct ISA_Operation op) {
    *op.operationRegisters[0] = *op.operationRegisters[1] + *op.operationRegisters[2];
}

void isa_and(struct ISA_Operation op) {
    *op.operationRegisters[0] = *op.operationRegisters[1] & *op.operationRegisters[2];
}

void isa_multiply(struct ISA_Operation op) {
    *op.operationRegisters[0] = *op.operationRegisters[1] * *op.operationRegisters[2];
}

void isa_str(struct ISA_Operation op) {
    *op.operationRegisters[0] = *op.operationRegisters[1];
}

void isa_jump(struct ISA_Operation op) {
    *op.systemReservedRegisters[PROGRAM_COUNTER_REGISTER_ADDRESS] = *op.operationRegisters[0];
}

const keywordFunc isa_instruction_map[NUMBER_OF_ISA_INSTRUCTIONS] = {
    isa_add,
    isa_add,
    isa_and,
    isa_and,
    isa_multiply,
    isa_multiply,
    isa_str,
    isa_str,
    isa_jump
};
