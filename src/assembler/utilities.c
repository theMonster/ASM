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

char* convertValueToBinaryString(int value, size_t length) {
    char *binary_s = malloc(sizeof(char) * length);

    for (int i = 0; i < length; ++i) {
        binary_s[length - 1 - i] = (value % 2) + '0';
        value /= 2;
    }

    return binary_s;
}

