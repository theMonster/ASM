//
//  asm.c
//  ASM
//
//  Created by Caleb on 7/6/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include "asm.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "asm-private.h"
#include "codegen.h"
#include "interpreter.h"
#include "preprocessor.h"

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
    const size_t MAX_ADDRESSABLE_MEMORY_COUNT = powf(2, isa_bit_count);
    void* memory[MAX_ADDRESSABLE_MEMORY_COUNT];
    const size_t MAX_LINE_SIZE = 256;
    char line[MAX_LINE_SIZE];
    size_t numberOfInstructions = 0;
    while (fgets(line, sizeof(line), f) != NULL && numberOfInstructions < MAX_ADDRESSABLE_MEMORY_COUNT) {
        int opCode = 0;
        char *originalLine = malloc(sizeof(line));
        strcpy(originalLine, line);
        char *lineCpy = preProcessAssemblyCode(line, MAX_ADDRESSABLE_MEMORY_COUNT, memory);
        if (lineCpy && strlen(lineCpy) > 0) {
            char *byteCode;
            translateAssemblyToByteCode(lineCpy, &byteCode, &opCode);
            struct Instruction* newInstruction = malloc(sizeof(struct Instruction));
            *newInstruction = (struct Instruction){ opCode, originalLine, byteCode };
            memory[numberOfInstructions++] = newInstruction;
        }
    }
    fclose(f);
    
    // execute byte code
    for (size_t i = 0; i < numberOfInstructions; ++i) {
        isa_register_t *programCounter = reservedRegisters[PROGRAM_COUNTER_REGISTER_ADDRESS];
        isa_register_t code = *reservedRegisters[MACHINE_STATUS_REGISTER_ADDRESS];
        // check the system's status byte for errors
        if (code != 0) {
            char *title, *message;
            retrieveInfoForStatusCode(code, &title, &message);
            printf("ERROR <code: %i>: %s. \"%s\"\n", code, title, message);
            break;
        }
        // get the next command from memory and validate that it is an instruction.
        void* potentiallyTheNextInstruction = memory[*programCounter];
        if (!potentiallyTheNextInstruction || !((struct Instruction *)potentiallyTheNextInstruction)->byteCode) {
            break;
        }
        // we now know it's an instruction. Validate it.
        struct Instruction instruction = *((struct Instruction *)potentiallyTheNextInstruction);
        if (!instruction.originalAsm || strlen(instruction.originalAsm) <= 0) {
            break;
        }
        printf("%s", instruction.originalAsm);
        // move the program counter
        // note: we move the program counter _before_ we execute a command so that we don't override a modification to the
        //       reserved memory register.
        *programCounter = *programCounter + 1;
        // THEN execute.
        executeByteCode(instruction, registersCount, generalPurposeRegisters, registersCount, reservedRegisters);
    }
    
    // free the memory.
    // we do this after we've executed because it's possible to "jump" between operations.
    for (size_t i =  0; i < MAX_ADDRESSABLE_MEMORY_COUNT; ++i) {
        void* memorySegment = memory[i];
        
        if (i < numberOfInstructions) {
            struct Instruction* instruction = (struct Instruction *)memorySegment;
            // free the memory
            free((void*)instruction->byteCode);
            free((void*)instruction->originalAsm);
            free(instruction);
        }
        // TODO: free other types of memory
    }
    
    printf("\n\n---- Registers ----\n");
    for (int i = 0; i < registersCount; ++i) {
        isa_register_t gr_v = *generalPurposeRegisters[i];
        isa_register_t rr_v = *reservedRegisters[i];
        printf("GR%i = %3i | SR%i = %3i;\n", i, gr_v, i, rr_v);
        free(generalPurposeRegisters[i]);
    }
}
