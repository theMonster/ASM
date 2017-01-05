//
//  preprocessor.h
//  ASM
//
//  Created by Jonassaint, Caleb on 12/24/16.
//  Copyright © 2016 theMonster. All rights reserved.
//

#ifndef preprocessor_h
#define preprocessor_h

#include <stdio.h>
#include "utilities.h"

struct PreProcessorLabelDefinition {
    char* name;         // for address labels, e.g "myLabel: ADD R0 R1 R2" (myLabel is the name)
    size_t address;     // for address labels, e.g "myLabel: ADD R0 R1 R2" this is the pc offset for the label
    char* type;         // for literal labels, e.g "myLabel: .stringz "a value goes here" (char *value = ".stringz"; would be the type)
    char* value;        // for literal labels, e.g "myLabel: .stringz "a value goes here" (char *value = ""a value goes here""; would be the value)
    size_t valueLen;
    // NOTE: Since the codegenerator can create labels too (only for unknown tokens in the immutable
    //       make sure we can distinguish between them. That way we know when an "anticipated" token is created.
    bool anticipatedLabelWithValueGeneratedByCodeGen;
};

struct PreProcessorContext {
    uint8_t errorCode;
    uint8_t errorAtCharacterIndex;
    size_t numberOfLabels;
    size_t numberOfValues;
    size_t memSize;
    struct PreProcessorLabelDefinition** labels;
    int* tempPassStorage;
};

extern void initPreProcessorContext(struct PreProcessorContext *context, size_t memSize);
extern void deinitPreProcessorContext(struct PreProcessorContext *context, void** memory);
extern char* preProcessAssemblyCode(struct PreProcessorContext *context, size_t bufferSize, uintmax_t lineAddress, char* assemblyCode);
extern void retrieveInfoForPreProcessorStatusCode(uint8_t code, char** title, char** message);

#endif /* preprocessor_h */
