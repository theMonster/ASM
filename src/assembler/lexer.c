//
//  lexer.c
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "asm.h"

char* convertValueToBinaryString(int value, size_t length) {
    char *binary_s = malloc(sizeof(char) * length);

    for (int i = 0; i < length; ++i) {
        binary_s[length - 1 - i] = (value % 2) + '0';
        value /= 2;
    }

    return binary_s;
}

const char* getByteCodeForToken(const char *token) {
    const char *byteCode = "";

    // check conversions:
    // ... immediate
    // ... registers
    if (token[0] == 'R') {
        // move all characters down by 1 (moving 'r' off)
        ++token;
        byteCode = convertValueToBinaryString(atoi(token), isa_register_size);
    }
    // ... opcode
    if (strcmp(token, "ADD") == 0) {
        byteCode = "001";
    }
    // throw exception if it's non of these "unrecognized token"
    

    return byteCode;
}


const char* translate_assembly_to_byte_code(char *assemblyCode) {
    // TODO: Translate to byte code equivilant
    char* token;
    assemblyCode = strdup(assemblyCode);
    while ((token = strsep(&assemblyCode, " ")) != NULL)
    {
        printf("%s\n", getByteCodeForToken(token));
    }

    free(assemblyCode);

    fflush(stdout);
    return "";
}

