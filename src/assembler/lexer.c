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

char* getByteCodeForToken(const char *token) {
    char *byteCode = "";

    // check conversions:
    // ... immediate
    // ... registers
    if (token[0] == 'R') {
        // move all characters down by 1 (moving 'r' off)
        ++token;
        byteCode = convertValueToBinaryString(atoi(token), isa_register_size);
    } else {
        // ... opcode
        const uint64_t lengthOfToken = strlen(token);
        for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
            // substring of token with range
            char *substring = malloc(lengthOfToken);
            strncpy(substring, isa_grammar[i], lengthOfToken);
            if (strcmp(token, substring) == 0) {
                byteCode = convertValueToBinaryString(i + 1, isa_opcode_size);
            } else {
                // throw exception if it's non of these "unrecognized token"
                byteCode = "\"UNRECOGNIZED_TOKEN\"";
            }
            free(substring);
        }
    }
    

    return byteCode;
}


const char* translate_assembly_to_byte_code(char *assemblyCode) {
    // TODO: Translate to byte code equivilant
    char* token;

    assemblyCode = strdup(assemblyCode);
    while ((token = strsep(&assemblyCode, " ")) != NULL)
    {
        printf("%s ", getByteCodeForToken(token));
    }

    free(assemblyCode);

    fflush(stdout);
    return "";
}

