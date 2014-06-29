//
//  parser.c
//  ARM
//
//  Created by Caleb Jonassaint on 6/28/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <string.h>
#include "parser.h"

//void cleanString(char *s) {
//    for (int i = 0; i < strlen(s); ++i) {
//        s[i] = '\0';
//    }
//}
//
//void append(char* s, char c) {
//    size_t len = strlen(s);
//    s[len] = c;
//    s[len+1] = '\0';
//}
//
//void parseIntoWords(char *s, callback c) {
//    char currentChar = '\0';
//    char buffer[100];
//    int i = 0;
//
//    do {
//        currentChar = s[i];
//        if (currentChar != ' ') append(buffer, currentChar);
//
//        if (currentChar == ' ' || currentChar == '\n' || currentChar == '\0') {
//            c(buffer);
//            // flush the buffer
//            cleanString(buffer);
//        }
//        ++i;
//    } while (currentChar != '\0');
//}