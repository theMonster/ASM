//
//  lc3.c
//  ARM
//
//  Created by Caleb on 7/1/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "lc3.h"

/* define these */
const uint64_t isa_bit_count = 16;
const uint64_t isa_opcode_size = 4;
const uint64_t isa_register_size = 3;

const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS] = {
    "ADD 0 DR SR SR"
};

void add(regsiter_t* registers) {
    registers[0] = registers[1] + registers[2];
}

const keywordFunc isa_instruction_map[NUMBER_OF_ISA_INSTRUCTIONS] = {
    add
};


/* implicit values */
uint64_t isa_registers_count() {
    return powl(2, isa_register_size);
}

