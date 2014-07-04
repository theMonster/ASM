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

char* getByteCodeForToken(const char *token, const char *grammar) {
    char *byteCode = malloc(strlen(token));

    // check conversions:
    // ... immediate
    // ... registers
    if (token[0] == 'R') {
        // move all characters down by 1 (moving 'r' off)
        ++token;

        char *binaryString = convertValueToBinaryString(atoi(token), isa_register_size);
        strncpy(byteCode, binaryString, 4);
        free(binaryString);
    } else {
        // ... opcode
        const uint64_t lengthOfToken = strlen(token);
        for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
            // substring of token with range
            char *grammarOpcode = malloc(lengthOfToken);
            strncpy(grammarOpcode, &grammar[i], lengthOfToken);
            if (strcmp(token, grammarOpcode) == 0) {
                char *binaryString = convertValueToBinaryString(i + 1, isa_opcode_size);
                strncpy(byteCode, binaryString, 4);
                free(binaryString);
            } else {
                // throw exception if it's non of these "unrecognized token"
                printf("\"UNRECOGNIZED_TOKEN\" (%s)", token);
                exit(EXIT_FAILURE);
            }
            free(grammarOpcode);
        }
    }

    return byteCode;
}

char* translate_assembly_to_byte_code(char *assemblyCode, int *grammar_index) {
    char *token;
    char *byteCode = malloc(isa_bit_count);
    const char *grammar = isa_grammar[0]; // TODO: attain dynamically

    *grammar_index = 0; // TODO: attain dynamically

    assemblyCode = strdup(assemblyCode);
    int bitsTraveled = 0;
    int charactersTraveled = 0;

    // check if grammar begins with void bits
    if (grammar[0] == '0') {
        // insert void bits into bytecode
        const char *voidBitPtr = grammar;
        int numberOfVoidBits = 0;
        // count the void bits (increment through the char* until you don't see '0' anymore)
        while (*(voidBitPtr++) == '0') ++numberOfVoidBits;

        // inject void bits into assembly code
        for (int i = 0; i < numberOfVoidBits; ++i) {
            byteCode[bitsTraveled + i] = '0';
        }
        bitsTraveled += numberOfVoidBits;
        grammar += numberOfVoidBits + 1;
    }

    while ((token = strsep(&assemblyCode, " ")) != NULL)
    {
        // check if bits should be injected
        if (grammar[charactersTraveled + 1] == '0') {
            const char *voidBitPtr = &grammar[charactersTraveled + 2];
            int numberOfVoidBits = 1; // we know there's at least 1
            // count the void bits (increment through the char* until you don't see '0' anymore)
            while (*(voidBitPtr++) == '0') ++numberOfVoidBits;

            // inject void bits into assembly code
            for (int i = 0; i < numberOfVoidBits; ++i) {
                byteCode[bitsTraveled + i] = '0';
            }
            bitsTraveled += numberOfVoidBits;
        }

        // do a regular
        char *a = getByteCodeForToken(token, grammar);
        for (int i = 0; i <= strlen(a); ++i) {
            byteCode[bitsTraveled + i] = a[i];
        }
        free(a);

        // update the counts
        charactersTraveled += strlen(token);
        bitsTraveled += strlen(a);
    }
    printf("%s\n", byteCode);
    free(assemblyCode);
    return byteCode;
}

