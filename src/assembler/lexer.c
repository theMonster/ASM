//
//  lexer.c
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "def.h"
#include "lexer.h"
#include "parser.h"




const char* getByteCodeForToken(const char *token) {
    char *byteCode = "";

    // check if it's a register
    // ... immutable
    // ... opcode
    // throw exception if it's non of these "unrecognized token"

    if (token[0] == 'R') {
        // move all characters down by 1 (moving 'r' off)
        token = &token[1];
        byteCode = "asdf";
    }



//    const char
//    for (int i = 0; i < ; <#increment#>) {
//        <#statements#>
//    }

    if (strcmp(token, "ADD") == 0) {
        byteCode = "001";
    }

    

    return byteCode;
}


const char* translate_assembly_to_byte_code(char *assemblyCode) {
    // TODO: Translate to byte code equivilant
    char* token;
    assemblyCode = strdup(assemblyCode);
    while ((token = strsep(&assemblyCode, " ")) != NULL)
    {
        printf("%s\n", getByteCodeForToken(token));
    }

    free(assemblyCode);

    fflush(stdout);
    return "";
}
