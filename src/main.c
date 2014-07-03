//
//  main.c
//  ARMSim
//
//  Created by Caleb on 6/25/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "asm.h"

int main() {
    regsiter_t a = 0;
    regsiter_t b = 5;
    regsiter_t c = 5;

    //             001 000 001 000 010
    char *assembly = "ADD R0 R1 R2";
    printf("%s\n", assembly);
    translate_assembly_to_byte_code(assembly);

    regsiter_t registers[] = {a,b,c};

    add(registers);


//    printf("%i\n", registers[0]);

//    getAddOperation();
////    add.instructionAnatomy = ["", "", "", "", "", "", "", "", "", ""]
//    int a1 = 2;
//    struct Register a;
//    a.value = &a1;
//    struct Register b;
//    b.value = &a1;
//    struct Register c;
//    c.value = &a1;
//
//    struct Register* registers[] = {a,b,c};
//    add(registers);
    return 0;
}
