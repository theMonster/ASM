//
//  codegen.c
//  ASM
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

char* getByteCodeForToken(const char *token, const char *grammar, const int opCode) {
    // check conversions:
    if (token[0] == '#' || (token[0] == '0' && token[1] == 'b') || (token[0] == '0' && token[1] == 'x')) {
        while (*(grammar - 3) != 'I') ++grammar;
        int numberOfBitsForImmediateValue = atoi(grammar);
        if (token[0] == '#') {
            // ... Immediate Integer Value
            ++token;
            // for now, we'll just assume the immediate value is the last argument
            int n = atoi(token);
            return convertValueToBinaryString(n, numberOfBitsForImmediateValue);
        }
        else if (token[0] == '0' && token[1] == 'b') {
            // ... Immediate Binary Value
            token += 2;
            size_t tokenLength = strlen(token);
            // since it's already in binary, we literally don't have to do anything except to pre-append the necessary 0's.
            char *tmp = malloc(numberOfBitsForImmediateValue);

            for (int i = 0; i < numberOfBitsForImmediateValue; ++i) {
                if (i < tokenLength) tmp[numberOfBitsForImmediateValue - 1 - i] = token[tokenLength - 1 - i];
                else tmp[numberOfBitsForImmediateValue - 1 - i] = '0';
            }
            return tmp;
        }
    } else if (token[0] == 'R') {
        // ... registers
        // move all characters down by 1 (moving 'r' off)
        ++token;

        return convertValueToBinaryString(atoi(token), isa_register_size);
    } else {
        // ... opcode
        // generate byte code for op code
        return convertValueToBinaryString(opCode, isa_opcode_size);
    }
    
    return NULL;
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
            }
            else if (*grammar == 'I') {
                // immediate value...
                while (*((++grammar - 1)) != ' ' && *grammar != '\0');
                while (*((++assemblyCode - 1)) != ' ' && *assemblyCode != '\0');
            }
            else if (*grammar == *assemblyCode) {
                // opcode
                while (*(++grammar) == *(++assemblyCode));
            }
            else {
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

void translateAssemblyToByteCode(char *assemblyCode, char** result, int *opCode) {
    char *byteCode = malloc(isa_bit_count);
    size_t asmLineLen = strlen(assemblyCode);
    char *assemblyCodeCopy = malloc(asmLineLen);
    memcpy(assemblyCodeCopy, assemblyCode, asmLineLen);
    // validate assembly:
    if (!assemblyCode || strlen(assemblyCode) == 0) {
        return;
    }
    
    *opCode = findGrammarIndex(assemblyCodeCopy);
    // did the grammar successfully get reported
    if (*opCode < 0) {
        if (*assemblyCodeCopy == '\n') return; // fail silently... (it's just whitespace)
        free(byteCode);
        free(assemblyCodeCopy);
        return;
    }

    const char* grammar = isa_grammar[*opCode];
    
    assemblyCodeCopy = strdup(assemblyCode);
    char *grammarCopy = strdup(grammar);
    char *assemblyToken, *grammarToken;
    
    while (assemblyCodeCopy && grammarCopy) {
        // get the tokens
        assemblyToken = strsep(&assemblyCodeCopy, " ");
        grammarToken = strsep(&grammarCopy, " ");
        
        // inject void bits:
        while (grammarToken && *grammarToken == '0') {
            byteCode = append(byteCode, grammarToken);
            // move to next grammar token
            grammarToken = strsep(&grammarCopy, " ");
        }
        
        char *byteCodeForToken = getByteCodeForToken(assemblyToken, grammarToken, *opCode);
        byteCode = append(byteCode, byteCodeForToken);
    }
    
    free(assemblyCodeCopy);
    free(grammarCopy);
    // check our work:
    const size_t byteCodeLen = strlen(byteCode);
    if (byteCodeLen != isa_bit_count) {
        printf("FATAL: Code Generator failed to generate proper byte count (%zu should be %i) for given asm: \"%s\" and grammar: \"%s\". Result = \"%s\" opcode = \"%i\".\n", byteCodeLen, isa_bit_count, assemblyCode, grammar, byteCode, *opCode);
        exit(EXIT_FAILURE);
    }
    *result = byteCode;
}

