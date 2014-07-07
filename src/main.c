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
    FILE *f;
    f = fopen("sample.asm", "r");
    if (!f) perror("Error opening file");

    interpretAndExecuteFile(f);
    return 0;
}
