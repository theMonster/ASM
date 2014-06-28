//
//  lexer.c
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>

void cleanString(char *s) {
    for (int i = 0; i < strlen(s); ++i) {
        s[i] = '\0';
    }
}

void append(char* s, char c)
{
    size_t len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

const char* translate_assembly_to_byte_code(const char *assemblyCode) {
    // TODO: move parsing code to parser file
    char currentChar;
    char buffer[10];
    int i = 0;

    do {
        currentChar = assemblyCode[i++];
        append(buffer, currentChar);


        if (currentChar == ' ' || currentChar == '\n' || currentChar == '\0') {
            printf("- %s\n", buffer);

            // TODO: Translate to byte code equivilant

            // flush the buffer
            cleanString(buffer);
        }

    } while (currentChar != '\0');


    fflush(stdout);
    return "";
}
