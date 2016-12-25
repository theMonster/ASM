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
        else if (token[0] == 'b' && token[1] == 'b') {
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
    // the number of successfully run loops determines which grammar is the best for this asm code.
    int loopCounts[NUMBER_OF_ISA_INSTRUCTIONS];
    // we begin by looping through each grammar.
    for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
        const char *grammar = isa_grammar[i];
        char *grammarCopy = strdup(grammar), *assemblyCodeCopy = strdup(asmCode);
        char *assemblyToken, *grammarToken;
        
        // then calculating the accuracy of each token.
        int loopCount = 0;
        while (assemblyCodeCopy && grammarCopy) {
            /// get the tokens:
            // these while loops move through each "string seperated by a space char", until it finds one is valid.
            // assemblyToken criteria: is that the length of the token is greater than 0.
            while (strlen(assemblyToken = strsep(&assemblyCodeCopy, " ")) <= 0);
            // grammarToken criteria: is that the length of the token is > 0 AND that it does not begin with a '0' or "void bit".
            // we skip void bits since they have no relevance to actual assembly code.
            while (strlen(grammarToken = strsep(&grammarCopy, " ")) <= 0 || grammarToken[0] == '0');

            // begin the matching process
            // on the first token, we need to check the op codes:
            if (loopCount == 0 && strcmp(grammarToken, assemblyToken) != 0) {
                break;
            }
            /// everything after the first token is basically "free range" meaning there' no designated order.
            // is this supposed to be a register? (grammar's that define a register, can put a single char in front of an R):
            else if (grammarToken[1] == 'R' && assemblyToken[0] != 'R') {
                break;
            }
            // is this supposed to be a immutable value?:
            //
            else if (grammarToken[0] == 'I' && (assemblyToken[0] != '#')) {
                break;
            }
            // increment the loop count since our test for validating these tokens passed.
            ++loopCount;
        }
        // set our loop count
        loopCounts[i] = loopCount;
    }
    // find the grammar who had the best loop count:
    int bestIndex = -1;
    int bestLoopCount = 0;
    for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
        int contestingLoopCount = loopCounts[i];
        if (contestingLoopCount > bestLoopCount) {
            bestIndex = i;
            bestLoopCount = contestingLoopCount;
        }
    }
    
    return bestIndex;
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
        printf("FATAL: Code Generator failed to find a grammar for given asm: \"%s\". Result = \"%s\".\n", assemblyCode, byteCode);
        free(byteCode);
        return;
    }

    const char* grammar = isa_grammar[*opCode];
    
    assemblyCodeCopy = strdup(assemblyCode);
    char *grammarCopy = strdup(grammar);
    char *assemblyToken, *grammarToken;
    
    while (assemblyCodeCopy && grammarCopy) {
        /// get the tokens:
        // these while loops move through each "string seperated by a space char", until it finds one is valid.
        // criteria is that the length of the token is greater than 0.
        while (strlen(assemblyToken = strsep(&assemblyCodeCopy, " ")) <= 0);
        while (strlen(grammarToken = strsep(&grammarCopy, " ")) <= 0);
        
        // inject void bits:
        while (grammarToken && *grammarToken == '0') {
            byteCode = append(byteCode, grammarToken);
            // move to next grammar token
            grammarToken = strsep(&grammarCopy, " ");
        }
        
        char *byteCodeForToken = getByteCodeForToken(assemblyToken, grammarToken, *opCode);
        byteCode = append(byteCode, byteCodeForToken);
    }
    
    free(grammarCopy);
    // check our work:
    const size_t byteCodeLen = strlen(byteCode);
    if (byteCodeLen != isa_bit_count) {
        printf("FATAL: Code Generator failed to generate proper byte count (%zu should be %i) for given asm: \"%s\" and grammar: \"%s\". Result = \"%s\" opcode = \"%i\".\n", byteCodeLen, isa_bit_count, assemblyCode, grammar, byteCode, *opCode);
        exit(EXIT_FAILURE);
    }
    *result = byteCode;
}

