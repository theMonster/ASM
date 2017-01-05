//
//  lc3.c
//  ARM
//
//  Created by Caleb on 7/1/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "lc3.h"

#define LC3_Throws

/* define these */
const size_t isa_max_address_size = 9;
const uint16_t isa_bit_count = 16;
const uint16_t isa_opcode_size = 4;
const uint16_t isa_register_size = 3;
/// system reserved registers <public use> (required by all ISAs)
const isa_register_t MACHINE_STATUS_REGISTER_ADDRESS = 0;
const isa_register_t IMMUTABLE_VALUE_REGISTER_ADDRESS = 7;
const isa_register_t PROGRAM_COUNTER_REGISTER_ADDRESS = 2;
/// system reserved registers <internal use>

// all grammars need to add up to the isa_bit_count
const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS] = {
    "ADD DR SR 000 SR",
    "ADD DR SR IMM6",
    "AND DR SR 000 SR",
    "AND DR SR IMM6",
    "MUL DR SR 000 SR",
    "MUL DR SR 0 IMM5",
    "ST DR 0 IMM8",
    "ST DR 0000000 SR",
    "JMP IMM12",
    "DIV DR SR 000 SR",
    "DIV DR SR 0 IMM5",
//    "BR*** IMM9",
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

void LC3_Throws isa_divide(struct ISA_Operation op) {
    const double divisor = *op.operationRegisters[2];
    if (divisor > 0) {
        *op.operationRegisters[0] = *op.operationRegisters[1] / divisor;
    }
    else {
        // cannot divide by `divsor <= 0`. ERROR<code: 009>: BAD_Arithmetic.
        *op.systemReservedRegisters[MACHINE_STATUS_REGISTER_ADDRESS] = 9;
    }
}

void isa_brnzp(struct ISA_Operation op) {
    
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
    isa_jump,
    isa_divide,
    isa_divide,
};


/*! LC-3 Machine Status Codes:
 * ----------------------------
 * 000: Machine_OK. "Everything is OK."
 * 009: EXC_Bad_Arithmetic. "This operation is not permitted. Operations like Dividing by 0 are not permitted."
 */
void retrieveInfoForStatusCode(isa_register_t code, char** title, char** message) {
    switch (code) {
        case 0:
            *title = "Machine_OK";
            *message = "Everything is OK.";
            break;
        case 9:
            *title = "EXC_Bad_Arithmetic";
            *message = "This operation is not permitted. Operations like Dividing by 0 are not permitted.";
            break;
        default:
            *title = "Unknown";
            *message = "Unknown Error";
            break;
    }
}
