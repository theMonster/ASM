//
//  utilities.c
//  ASM
//
//  Created by Caleb on 7/5/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "preprocessor.h"

void addLabelToContext(void *preProcessorContext, void* newLabel) {
    struct PreProcessorContext* context = preProcessorContext;
    // resize the array
    const size_t newNumberOfLabelsSize = context->numberOfLabels + 1;
    context->labels = realloc(context->labels, sizeof(struct PreProcessorLabelDefinition *) * newNumberOfLabelsSize);
    context->labels[context->numberOfLabels] = newLabel;
    context->numberOfLabels = newNumberOfLabelsSize;
}

extern void* findLabelInContext(void *preProcessorContext, const char* name) {
    struct PreProcessorContext* context = preProcessorContext;
    for (int i = 0; i < context->numberOfLabels; ++i) {
        struct PreProcessorLabelDefinition* label = context->labels[i];
        if (strcmp(name, label->name) == 0) {
            return label;
        }
    }
    return NULL;
}

bool isOpCode(const char* str, size_t len) {
    for (int i = 0; i < NUMBER_OF_ISA_INSTRUCTIONS; ++i) {
        if (strncmp(str, isa_grammar[i], len) == 0) {
            return true;
        }
    }
    return false;
}

isa_register_t convertBinaryStringToInt(char *binaryString, uint64_t numberOfBitsInString) {
    isa_register_t value = 0;

    for (int i = 0; i < numberOfBitsInString; ++i) {
        char *s = binaryString + (numberOfBitsInString - 1 - i);
        int bit = *s - '0';
        value |= bit  << i;
    }

    return value;
}

char* convertValueToBinaryString(uint64_t value, size_t length) {
    char *binaryString = malloc(sizeof(char) * (length + 1));
    memset(binaryString, '\0', length);
    
    for (int i = (int)length - 1; i < length && i >= 0; --i) {
        binaryString[i] = (value % 2) + '0';
        value /= 2;
    }
    
    return binaryString;
}
