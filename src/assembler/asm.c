//
//  asm.c
//  ASM
//
//  Created by Caleb on 7/6/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void allocateRegisters(isa_register_t* registers[], size_t registersCount) {
    for (int i = 0; i < registersCount; ++i) {
        isa_register_t *value = malloc(sizeof(isa_register_t));
        *value = 0;
        registers[i] = value;
    }
}

void interpretAndExecuteFile(FILE *f) {
    size_t registersCount = powf(2, isa_register_size);
    isa_register_t* reservedRegisters[registersCount];
    isa_register_t* generalPurposeRegisters[registersCount];
    
    allocateRegisters(reservedRegisters, registersCount);
    allocateRegisters(generalPurposeRegisters, registersCount);

    // generate byte code
    const size_t MAX_NUMBER_OF_COMMANDS = powf(2, isa_bit_count);
    struct Instruction instructions[MAX_NUMBER_OF_COMMANDS];
    const size_t MAX_LINE_SIZE = 64;
    char line[MAX_LINE_SIZE];
    for (size_t i = 0; fgets(line, sizeof(line), f) != NULL && i < MAX_NUMBER_OF_COMMANDS; ++i) /* read a line */
    {
        int opCode = 0;
        char* lineCpy = malloc(MAX_LINE_SIZE);
        memcpy(lineCpy, line, MAX_LINE_SIZE);
        char *byteCode;
        translateAssemblyToByteCode(lineCpy, &byteCode, &opCode);
        if (byteCode) {
            struct Instruction newInstruction = { opCode, lineCpy, byteCode };
            instructions[i] = newInstruction;
        }
    }
    fclose(f);
    
    // execute byte code
    for (size_t i = 0; i < MAX_NUMBER_OF_COMMANDS; ++i) {
        isa_register_t *programCounter = reservedRegisters[PROGRAM_COUNTER_REGISTER_ADDRESS];
        isa_register_t code = *reservedRegisters[MACHINE_STATUS_REGISTER_ADDRESS];
        // check the system's status byte for errors
        if (code != 0) {
            char *title, *message;
            retrieveInfoForStatusCode(code, &title, &message);
            printf("ERROR <code: %i>: %s. \"%s\"\n", code, title, message);
        }
        // execute the next command
        struct Instruction instruction = instructions[*programCounter];
        if (!instruction.originalAsm || strlen(instruction.originalAsm) <= 0) {
            break;
        }
        printf("%s", instruction.originalAsm);
        // move the program counter
        *programCounter = *programCounter + 1;
        executeByteCode(instruction, registersCount, generalPurposeRegisters, registersCount, reservedRegisters);
    }
    
    // free the operations.
    // we do this after we've executed because it's possible to jump between operations.
    for (size_t i = 0; i < MAX_NUMBER_OF_COMMANDS; ++i) {
        struct Instruction instruction = instructions[i];
        // if it's null, we assume this is the end.
        if (!instruction.originalAsm || strlen(instruction.originalAsm) <= 0) {
            break;
        }
        else {
            // free the memory
            free((void*)instruction.byteCode);
            free((void*)instruction.originalAsm);
        }
    }
    
    printf("\n\n---- Registers ----\n");
    
    for (int i = 0; i < registersCount; ++i) {
        isa_register_t gr_v = *generalPurposeRegisters[i];
        isa_register_t rr_v = *reservedRegisters[i];
        printf("GR%i = %i | SR%i = %i;\n", i, gr_v, i, rr_v);
        free(generalPurposeRegisters[i]);
    }
    
}
