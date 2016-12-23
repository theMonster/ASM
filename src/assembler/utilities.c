//
//  utilities.c
//  ASM
//
//  Created by Caleb on 7/5/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"

isa_register_t convertBinaryStringToInt(char *binaryString, uint64_t numberOfBitsInString) {
    isa_register_t value = 0;

    for (int i = 0; i < numberOfBitsInString; ++i) {
        char *s = binaryString + (numberOfBitsInString - 1 - i);
        int bit = *s - '0';
        value |= bit  << i;
    }

    return value;
}

char* convertValueToBinaryString(uint64_t value, size_t length) {
    char *binaryString = malloc(sizeof(char) * length);
    memset(binaryString, '0', length);
    
    for (size_t i = length - 1; i <= length + 1; --i) {
        binaryString[i] = (value % 2) + '0';
        value /= 2;
    }
    
    return binaryString;
}
