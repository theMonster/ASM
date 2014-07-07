//
//  lc3.c
//  ARM
//
//  Created by Caleb on 7/1/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "lc3.h"

/* define these */
const uint8_t isa_bit_count = 16;
const uint8_t isa_opcode_size = 4;
const uint8_t isa_register_size = 3;

// all grammars need to add up to the isa_bit_count
const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS] = {
    "ADD DR SR 000 SR",
    "ADD DR SR IMM6",
    "AND DR SR 000 SR",
    "AND DR SR IMM6"
};

// all instructions should take this parameter: (regsiter_t* registers)
void isa_add(isa_register_t* registers[]) {
    *registers[0] = *registers[1] + *registers[2];
}

void isa_and(isa_register_t* registers[]) {
    isa_register_t r1 = *registers[1];
    isa_register_t r2 = *registers[2];
    *registers[0] = r1 & r2;
}

const keywordFunc isa_instruction_map[NUMBER_OF_ISA_INSTRUCTIONS] = {
    isa_add,
    isa_add,
    isa_and,
    isa_and
};
