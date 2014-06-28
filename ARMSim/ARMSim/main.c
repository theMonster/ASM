//
//  main.c
//  ARMSim
//
//  Created by Caleb on 6/25/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include "definitions.h"

int main() {
    printf("Hello, World\n");

    getAddOperation();
//    add.instructionAnatomy = ["", "", "", "", "", "", "", "", "", ""]
    int a1 = 2;
    struct Register a;
    a.value = &a1;
    struct Register b;
    b.value = &a1;
    struct Register c;
    c.value = &a1;

    struct Register* registers[] = {a,b,c};
    add(registers);
    return 0;
}
