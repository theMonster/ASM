//
//  main.c
//  ARMSim
//
//  Created by Caleb on 6/25/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm.h"

int main() {
    isa_register_t a = 0;
    isa_register_t b = 5;
    isa_register_t c = 5;

    //             001 000 001 000 010
    char *assembly = "ADD R1 R1 R0";
    printf("%s\n", assembly);
    isa_register_t* registers[] = {&a,&b,&c};
    int *grammar_index = malloc(1);
    char *byteCode = translate_assembly_to_byte_code(assembly, grammar_index);

    executeByteCode(byteCode, *grammar_index, registers);

    // print registers
    printf("\n");
    int numberOfRegisters = sizeof(registers) / sizeof(isa_register_t*);
    for (int i = 0; i < numberOfRegisters; ++i) {
        printf("R%i = %i\n", i, *registers[i]);
    }

    return 0;
}
