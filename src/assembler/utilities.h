//
//  utilities.h
//  ASM
//
//  Created by Caleb on 7/5/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include "asm-private.h"

// declare boolean type and true/false
typedef char bool;
#define true 1
#define false 0

extern char* append(char * string1, char * string2);
extern isa_register_t convertBinaryStringToInt(char *binaryString, uint64_t numberOfBitsInString);
extern char* convertValueToBinaryString(uint64_t value, size_t length);

#endif
