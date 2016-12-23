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

void executeByteCode(Instruction instruction, size_t generalPurposeRegistersCount, isa_register_t* generalPurposeRegisters[], size_t reservedRegistersCount, isa_register_t* reservedRegisters[]) {
    // create copy of bytecode
    char* mutableByteCode = malloc(sizeof(char) * isa_bit_count);
    strcpy(mutableByteCode, instruction.byteCode);
    char* mutableGrammar = malloc(sizeof(char) * strlen(isa_grammar[instruction.opCode]));
    strcpy(mutableGrammar, isa_grammar[instruction.opCode]);

    // remove void bits
    int numberOfVoidBitsAtBeginning = 0;
    while (*mutableGrammar == '0' || *mutableGrammar == ' ') {
        if (*mutableGrammar == '0') ++numberOfVoidBitsAtBeginning;
        ++mutableGrammar;
    }
    mutableByteCode += numberOfVoidBitsAtBeginning;
    
    const size_t MAXIMUM_NUMBER_OF_REGISTERS_FOR_SINGLE_FUNCTION = 3;
    isa_register_t* operationRegisters[MAXIMUM_NUMBER_OF_REGISTERS_FOR_SINGLE_FUNCTION] = {};
    
    for (int i = 0; *mutableByteCode != '\0';) {
        // destory the void bits...
        if (*mutableGrammar == '0') {
            // void bits...
            while (*mutableGrammar == '0') {
                ++mutableByteCode;
                ++mutableGrammar;
            }
            ++mutableGrammar;
        }

        if (*(mutableGrammar + 1) == 'R') {
            // register...
            uint64_t pos = convertBinaryStringToInt(mutableByteCode, isa_register_size);
            isa_register_t *pointerToRegister = generalPurposeRegisters[pos];
            operationRegisters[i++] = pointerToRegister;
            mutableByteCode += isa_register_size;
        }
        else if (*mutableGrammar == 'I') {
            // immediate... move past the IMM
            mutableGrammar += 3;
            int numberOfBits = atoi(mutableGrammar);
            isa_register_t immediateValue = convertBinaryStringToInt(mutableByteCode, numberOfBits);
            // for now, we only ever use the first reserved register for the immediate value.
            reservedRegisters[IMMUTABLE_VALUE_REGISTER_ADDRESS] = &immediateValue;
            operationRegisters[i++] = reservedRegisters[IMMUTABLE_VALUE_REGISTER_ADDRESS];
            mutableByteCode += numberOfBits;
        }
        else {
            // opcode...
            mutableByteCode += isa_opcode_size;
        }
        
        // move to the next token
        while (*(++mutableGrammar - 1) != ' ' && *mutableGrammar != '\0');
    }
    
    struct ISA_Operation op = { operationRegisters, generalPurposeRegisters, reservedRegisters };
    isa_instruction_map[instruction.opCode](op);
}
