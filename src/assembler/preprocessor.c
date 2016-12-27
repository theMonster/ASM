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

// MARK: - Private
void checkPass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    
}

void allUpperCasePass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    if (*assembly >= 'a' && *assembly <= 'z') {
        *assembly = *assembly + ('A' - 'a');
    }
}

void removeNewLinePass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    char c = *assembly;
    if (c == '\n') {
        *emptyBufferByN = 1;
    }
}

void commentsPass(char *assmebly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    enum CommentType { CommentTypeNo = 0, CommentTypeNewLine = 1, CommentTypeMultiLine = 2 };
    char c = *assmebly, lastC = *(assmebly - 1), nextC = *(assmebly + 1);
    // *_were_* we looking at a comment and start ignoring it?
    if (*tmp == CommentTypeNewLine && c == '\n') {
        *emptyBufferByN = bufferLength;
        // reset flag
        *tmp = CommentTypeNo;
    }
    else if (*tmp == CommentTypeMultiLine && c == ';' && lastC == '*') {
        *emptyBufferByN = bufferLength;
        // reset flag
        *tmp = CommentTypeNo;
    }
    // is this a comment we're looking at?
    else if (c == ';' || *tmp > CommentTypeNo) {
        *addBufferToNewAsm = false;
        // flag for us to know that a comment is going on. (only set it if it hasn't already)
        if (*tmp == CommentTypeNo) {
            *tmp = nextC == '*' ? CommentTypeMultiLine : CommentTypeNewLine;
        }
    }
}

// MARK: - Public
const size_t NUMBER_OF_PRE_PROCESSOR_OPS = 3;

void initPreProcessorContext(struct PreProcessorContext *context) {
    context->numberOfLabels = 0;
    // temp storage for pre-processor operations
    context->tempPassStorage = malloc(sizeof(int) * NUMBER_OF_PRE_PROCESSOR_OPS);
}

void deinitPreProcessorContext(struct PreProcessorContext *context) {
    free(context->tempPassStorage);
}

char* preProcessAssemblyCode(struct PreProcessorContext *context, size_t bufferSize, char* assemblyCode, size_t memSize, void** memory) {
    size_t asmCodeLen = strlen(assemblyCode);
    char *newAssembly = malloc(bufferSize);
    
    int bufferIndex = 0, asmIndex = 0, newAsmIndex = 0;
    char buffer[bufferSize], *c;
    do {
        c = assemblyCode + asmIndex;
        // pre-processor flags
        bool addBufferToNewAsm = true;
        size_t emptyBufferByN = 0;
        
        // run preprocessor passes:
        size_t pass = 0;
        size_t bufferLength = bufferIndex + 1;
        commentsPass(c, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        removeNewLinePass(c, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        allUpperCasePass(c, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        
        // build buffer
        buffer[bufferIndex++] = *c;
        
        if (emptyBufferByN > 0) {
            // reset the index and length for future ops
            bufferIndex -= emptyBufferByN;
            bufferLength = bufferIndex + 1;
            memset(buffer + (bufferLength - emptyBufferByN), 0, emptyBufferByN);
        }
        
        if (addBufferToNewAsm) {
            // add the new buffer to the assmembly
            newAsmIndex += bufferIndex;
            strncpy(newAssembly, buffer, bufferLength);
        }
        
        // move to the next asmIndex
        ++asmIndex;
    }
    while (asmIndex < asmCodeLen);
    return newAssembly;
}
