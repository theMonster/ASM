//
//  lc3.h
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

/* define these */

const long isa_bit_count = 16;
const long isa_opcode_size = 4;
const long isa_register_size = 3;
typedef uint64_t regsiter_type_t;

const char* isa_grammar[] = {
    "ADD DR SR SR"
};

void add(regsiter_type_t* registers) {
    registers[0] = registers[1] + registers[2];
}

typedef void (*keywordFunc)(regsiter_type_t *registers);
const keywordFunc isa_instruction_map[] = {
    add
};

