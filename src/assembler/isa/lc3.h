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
extern const uint64_t isa_bit_count;
extern const uint64_t isa_opcode_size;
extern const uint64_t isa_register_size;
typedef uint16_t regsiter_t;

extern const char* isa_grammar[];

extern void add(regsiter_t* registers);


typedef void (*keywordFunc)(regsiter_t *registers);
extern const keywordFunc isa_instruction_map[];

/* implicit values */
extern uint64_t isa_registers_count();

#endif
