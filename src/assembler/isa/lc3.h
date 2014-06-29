//
//  lc3.h
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

/* define these */

const uint64_t isa_bit_count = 16;
const uint64_t isa_opcode_size = 4;
const uint64_t isa_register_size = 3;
typedef uint16_t regsiter_t;

const char* isa_grammar[] = {
    "ADD DR SR 000 SR"
};

void add(regsiter_t* registers) {
    registers[0] = registers[1] + registers[2];
}

typedef void (*keywordFunc)(regsiter_t *registers);
const keywordFunc isa_instruction_map[] = {
    add
};

