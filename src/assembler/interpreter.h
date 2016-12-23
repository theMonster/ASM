//
//  interpreter.h
//  ASM
//
//  Created by Caleb on 7/4/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#ifndef interpreter_h
#define interpreter_h

#include "asm.h"

typedef struct Instruction {
    const int opCode;
    const char* originalAsm;
    const char* byteCode;
} Instruction;

extern void executeByteCode(Instruction instruction, size_t generalPurposeRegistersCount, isa_register_t* genPurpRegisters[], size_t reservedRegistersCount, isa_register_t* reservedRegisters[]);

#endif
