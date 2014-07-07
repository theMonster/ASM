//
//  interpret.c
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "interpreter.h"

void executeByteCode(char *byteCode, int grammar_index, isa_register_t* genPurpRegisters[]) {
    // remove void bits
    char *grammar = malloc(strlen(isa_grammar[grammar_index]));
    strcpy(grammar, isa_grammar[grammar_index]);

    int numberOfVoidBitsAtBeginning = 0;
    while (*grammar == '0' || *grammar == ' ') {
        if (*grammar == '0') ++numberOfVoidBitsAtBeginning;
        ++grammar;
    }
    byteCode += numberOfVoidBitsAtBeginning;

    isa_register_t* registersForFunc[3] = {NULL, NULL, NULL}; // TODO: Attain dynamically
    int registerPos = 0;

    while (*byteCode != '\0') {
        // destory the void bits...
        if (*grammar == '0') {
            // void bits...
            while (*grammar == '0') {
                ++grammar;
            }
            ++grammar;
        }

        if (*(grammar + 1) == 'R') {
            // register...
            uint64_t pos = convertBinaryStringToInt(byteCode, isa_register_size);
            registersForFunc[registerPos] = genPurpRegisters[pos];
            byteCode += isa_register_size;
            ++registerPos;
        } else if (*grammar == 'I') {
            // immediate...
            grammar += 3;
            int numberOfBits = atoi(grammar);
            isa_register_t immediateValue = convertBinaryStringToInt(byteCode, numberOfBits);
            registersForFunc[registerPos] = &immediateValue;
            ++registerPos;
        } else {
            // opcode...
            byteCode += isa_opcode_size;
        }

        // move to the next token
        while (*(++grammar - 1) != ' ' && *grammar != '\0');
    }

    isa_instruction_map[grammar_index](registersForFunc);
}