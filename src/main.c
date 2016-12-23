//
//  main.c
//  ASM
//
//  Created by Jonassaint, Caleb on 12/22/16.
//  Copyright © 2016 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm.h"

int main(int argc, char** argv) {
    FILE *f;
    f = fopen("sample.asm", "r");
    if (!f) perror("Error opening file");
    
    interpretAndExecuteFile(f);
    return 0;
}
