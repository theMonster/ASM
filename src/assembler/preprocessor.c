//
//  preprocessor.c
//  ASM
//
//  Created by Jonassaint, Caleb on 12/24/16.
//  Copyright © 2016 theMonster. All rights reserved.
//

#include "preprocessor.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>

extern char* preProcessAssemblyCode(char* assemblyCode, size_t memSize, void** memory) {
    size_t asmCodeLen = strlen(assemblyCode);
    
    size_t startIndex = 0;
    size_t length = 0;
    for (; length < asmCodeLen; ++length) {
        char c = assemblyCode[length];
        if (c == ';' || c == '\n' || c == '\0') {
            // if ';' or null terminating char, its a comment... so... we're done here.
            break;
        }
        else if (c == ':') {
            // it's
            // restart the startIndex
            startIndex = length + 1;
        }
    }
    
    char* assemblyCopy = malloc(length + 1);
    memcpy(assemblyCopy, assemblyCode + startIndex, length - startIndex);
    
    return assemblyCopy;
}
