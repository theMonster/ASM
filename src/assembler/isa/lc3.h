//
//  lc3.h
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#ifndef lc3_h
#define lc3_h

#include <math.h>
#include <stdio.h>

/* define these */
#define NUMBER_OF_ISA_INSTRUCTIONS 5
extern const uint16_t isa_bit_count;
extern const uint16_t isa_opcode_size;
extern const uint16_t isa_register_size;
typedef uint16_t isa_register_t;

extern const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS];

typedef void (*keywordFunc)(isa_register_t* registers[], isa_register_t* generalPurposeRegisters[]);
extern const keywordFunc isa_instruction_map[NUMBER_OF_ISA_INSTRUCTIONS];

#endif
