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
#include "utilities.h"

char* getByteCodeForToken(const char *token, const char *grammar) {
    const uint64_t lengthOfToken = strlen(token);
    char *byteCode = malloc(lengthOfToken);

    // check conversions:
    if (token[0] == '#' || (token[0] == '0' && token[1] == 'b') || (token[0] == '0' && token[1] == 'x')) {
        while (*(grammar - 3) != 'I') ++grammar;
        int numberOfBitsForImmediateValue = atoi(grammar);
        if (token[0] == '#') {
            // ... Immediate Integer Value
            ++token;
            // for now, we'll just assume the immediate value is the last argument
            int n = atoi(token);
            byteCode = convertValueToBinaryString(n, numberOfBitsForImmediateValue);
        } else if (token[0] == '0' && token[1] == 'b') {
            // ... Immediate Binary Value
            token += 2;
            size_t tokenLength = strlen(token);
            // since it's already in binary, we literally don't have to do anything except to pre-append the necessary 0's.
            char *tmp = malloc(numberOfBitsForImmediateValue);

            for (int i = 0; i < numberOfBitsForImmediateValue; ++i) {
                if (i < tokenLength) tmp[numberOfBitsForImmediateValue - 1 - i] = token[tokenLength - 1 - i];
                else tmp[numberOfBitsForImmediateValue - 1 - i] = '0';
            }
            byteCode = tmp;
        }
    } else if (token[0] == 'R') {
        // ... registers
        // move all characters down by 1 (moving 'r' off)
        ++token;

        char *binaryString = convertValueToBinaryString(atoi(token), isa_register_size);
        strncpy(byteCode, binaryString, 4);
        free(binaryString);
    } else {
        // ... opcode
        for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS;) {
            // substring of token with range
            char *grammarOpcode = malloc(lengthOfToken);
            strncpy(grammarOpcode, &grammar[i], lengthOfToken);
            if (strcmp(token, grammarOpcode) == 0) {
                char *binaryString = convertValueToBinaryString(i + 1, isa_opcode_size);
                strncpy(byteCode, binaryString, 4);
                free(binaryString);
                free(grammarOpcode);
                break;
            } else {
                // throw exception if it's non of these "unrecognized token"
                printf("UNRECOGNIZED_TOKEN \"%s\"", token);
                exit(EXIT_FAILURE);
            }
            
        }
    }

    return byteCode;
}



int findGrammarIndex(const char *asmCode) {
    for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
        char *grammar = malloc(strlen(isa_grammar[i]));
        strcpy(grammar, isa_grammar[i]);

        const char *assemblyCode = asmCode;
        while (1) {

            if (*grammar != *assemblyCode && *grammar != 'I')
                break;

            if (*grammar == 'R') {
                // register...
                while (*((++grammar - 1)) != ' ' && *grammar != '\0');
                while (*((++assemblyCode - 1)) != ' ' && *assemblyCode != '\0');
                printf("");
            } else if (*grammar == 'I') {
                // immediate value...
                while (*((++grammar - 1)) != ' ' && *grammar != '\0');
                while (*((++assemblyCode - 1)) != ' ' && *assemblyCode != '\0');
            } else if (*grammar == *assemblyCode) {
                // opcode
                while (*(++grammar) == *(++assemblyCode));
            } else {
                break; // failed to match the token, we'll assume that this is no match.
            }

            // skip void bits
            while (*grammar == '0' || *grammar == '1') ++grammar;
            if (*grammar == ' ') ++grammar;

            // if 'grammar' ends on a SR or DR, remove the 'S' or 'D'
            if ((*grammar == 'S' || *grammar == 'D') && *(grammar + 1) == 'R')
                ++grammar;

            // if both grammar and assemblyCode have reached the end and are '\0', then we've found a match
            if (*grammar == '\0' && *assemblyCode == '\0') {
                return i;
            }

        }
    }
    return -1;
}

char* translate_assembly_to_byte_code(char *assemblyCode, int *grammar_index) {
    char *token;
    char *byteCode = malloc(isa_bit_count);

    *grammar_index = findGrammarIndex(assemblyCode);
    // did the grammar successfully get reported
    if (*grammar_index < 0) {
        if (*assemblyCode == '\n') return NULL; // fail silently... (it's just whitespace)
        printf("Failure to match a grammar for assebly code: \"%s\"", assemblyCode);
        exit(EXIT_FAILURE);
    }

    const char *grammar = isa_grammar[*grammar_index];

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
    free(assemblyCode);
    return byteCode;
}

