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

struct PreProcessorContext {
    size_t numberOfLabels;
    int* tempPassStorage;
};

extern void initPreProcessorContext(struct PreProcessorContext *context);
extern void deinitPreProcessorContext(struct PreProcessorContext *context);
extern char* preProcessAssemblyCode(struct PreProcessorContext *context, size_t bufferSize, char* assemblyCode, size_t memSize, void** memory);

#endif /* preprocessor_h */
