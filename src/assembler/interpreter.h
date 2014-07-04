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

extern void executeByteCode(char *byteCode, int grammar_index, isa_register_t* genPurpRegisters[]);

#endif
