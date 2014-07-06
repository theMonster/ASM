//
//  utilities.h
//  ASM
//
//  Created by Caleb on 7/5/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include "asm.h"

extern isa_register_t convertBinaryStringToInt(char *binaryString, uint64_t numberOfBitsInString);
extern char* convertValueToBinaryString(uint64_t value, size_t length);

#endif
