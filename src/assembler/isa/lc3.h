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
#define NUMBER_OF_ISA_INSTRUCTIONS 11
extern const uint16_t isa_bit_count;
extern const uint16_t isa_opcode_size;
extern const uint16_t isa_register_size;
typedef uint16_t isa_register_t;

extern const char* isa_grammar[NUMBER_OF_ISA_INSTRUCTIONS];

/// operation's (actual c functions that implement the instruction / operation)
struct ISA_Operation {
    isa_register_t** operationRegisters;
    isa_register_t** generalPurposeRegisters;
    isa_register_t** systemReservedRegisters;
};
typedef void (*keywordFunc)(struct ISA_Operation);
extern const keywordFunc isa_instruction_map[NUMBER_OF_ISA_INSTRUCTIONS];

/// system register indecies
extern const isa_register_t MACHINE_STATUS_REGISTER_ADDRESS;
extern const isa_register_t IMMUTABLE_VALUE_REGISTER_ADDRESS;
extern const isa_register_t PROGRAM_COUNTER_REGISTER_ADDRESS;

// used to get more info about a status code.
extern void retrieveInfoForStatusCode(isa_register_t code, char** title, char** message);

#endif
