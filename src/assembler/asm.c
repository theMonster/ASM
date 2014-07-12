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

void interpretAndExecuteFile(FILE *f) {
    size_t registersCount = powf(2, isa_register_size);
    isa_register_t* registers[registersCount];

    for (int i = 0; i < registersCount; ++i) {
        registers[i] = malloc(sizeof(isa_register_t));
        *registers[i] = 0;
    }

    char line[128]; /* or other suitable maximum line size */
    while (fgets(line, sizeof line, f) != NULL) /* read a line */
    {
        int grammar_index = 0;
        char *byteCode = translate_assembly_to_byte_code(line, &grammar_index);
        if (byteCode) executeByteCode(byteCode, grammar_index, registers);

        printf("%s", line);
    }
    fclose(f);

    // print registers
    printf("\n");
    size_t numberOfRegisters = sizeof(registers) / sizeof(isa_register_t*);
    for (int i = 0; i < numberOfRegisters; ++i) {
        printf("R%i = %i\n", i, *registers[i]);
    }

    // free the registers
    for (int i = 0; i < registersCount; ++i) {
        free(registers[i]);
    }
}
