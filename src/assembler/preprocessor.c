//
//  preprocessor.c
//  ASM
//
//  Created by Jonassaint, Caleb on 12/24/16.
//  Copyright © 2016 theMonster. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "asm-private.h"
#include "utilities.h"

// MARK: - Private
struct PreProcessorLabelDefinition* extractLabelsPass(struct PreProcessorContext* context, char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    char c = *assembly, nextC = *(assembly + 1);
    // this is temp storage for each token (seperated by a space) in the assembly code (note: this "scope" goes beyond each line).
    static char* lastToken = NULL;
    static struct PreProcessorLabelDefinition* lastValueLabel;
    
    // handle flag
    bool *ignoreTheRestOfThisLine = (bool *)tmp;
    if (*ignoreTheRestOfThisLine == true) {
        // reset the flag when we see the new line character:
        if (c == '\n') {
            *ignoreTheRestOfThisLine = false;
        }
        return NULL;
    }
    
    // capture initial label definition. (e.g 'myLabel:', 'myLabel')
    const char *bufferBeginPtr = assembly - (bufferLength - 1);
    const bool lineBeginsWithNonDelimiter = *bufferBeginPtr != ' ' && *bufferBeginPtr != '\n';
    const bool isInLiteralValue = lastToken && *lastToken == '.';
    const bool isCurrentCharDelimiter = (c == ' ' && nextC != ' ' && !isInLiteralValue) || c == ':' || c == '\n';
    const bool isValidTokenInBuffer = lineBeginsWithNonDelimiter && isCurrentCharDelimiter;
    if (isValidTokenInBuffer) {
        // we'll use this "begin ptr" in all cases so save it
        if (lastToken) {
            char *bufferBeginPtr = assembly - (bufferLength - 1);
            char currentToken[bufferLength];
            strncpy(currentToken, bufferBeginPtr, bufferLength);
            // alright, now we'll assign our label's value. Here are the rules:
            // 1. If the current token is an op code, then this is a address based label.
            if (isOpCode(currentToken, bufferLength) && !findLabelInContext(context, lastToken)) {
                // prepare the new token:
                struct PreProcessorLabelDefinition* label = malloc(sizeof(struct PreProcessorLabelDefinition));
                label->name = strdup(lastToken);
                lastToken = NULL;
                *ignoreTheRestOfThisLine = true;
                free(lastToken);
                return label;
            }
            // 2. If the current buffer is a type, then this is a literal based label.
            else if (*currentToken == '.') {
                // look for an existing token (happens when the code generator beats us to building a label.
                struct PreProcessorLabelDefinition* label = findLabelInContext(context, lastToken);
                if (!label) {
                    // if we failed to get an existing token, then allocate it.
                    label = malloc(sizeof(struct PreProcessorLabelDefinition));
                }
                label->name = strdup(lastToken);
                // assign the type to the lastToken var.
                lastToken = malloc(sizeof(char) * bufferLength);
                strncpy(lastToken, bufferBeginPtr, bufferLength - 1);
                label->type = strdup(lastToken);
                // we'll wait until the next token before we assign it.
                // empty the buffer and wait for the next delimeter.
                *emptyBufferByN = bufferLength;
                lastValueLabel = label;
                free(lastToken);
                // we use this flag again to see wether or not we should return this label so we'll create a new one.
                if (!label->anticipatedLabelWithValueGeneratedByCodeGen) {
                    return label;
                }
                else {
                    return NULL;
                }
            }
            // 2.5. If the last token begins with a period, we did just completed the type, which means this is the value.
            else if (*lastToken == '.') {
                // grab the last label and assign the value to it.
                struct PreProcessorLabelDefinition* label = lastValueLabel;
                const size_t valueLen = bufferLength - 1;
                label->value = malloc(sizeof(char) * valueLen);
                label->valueLen = valueLen;
                strncpy(label->value, bufferBeginPtr, valueLen);
                // check if we need to assign the address or if it's already been done.
                if (label->anticipatedLabelWithValueGeneratedByCodeGen) {
                    // reset the flag since we're now done "anticipating" it.
                    label->anticipatedLabelWithValueGeneratedByCodeGen = false;
                }
                else {
                    // we reassign the address since this is a variable.
                    label->address = context->memSize - context->numberOfValues++;
                }
                // clean up.
                lastToken = NULL;
                *emptyBufferByN = bufferLength;
                *ignoreTheRestOfThisLine = true;
                // no need to return the label since it's already been setup. This is just the "second" part.
                return NULL;
            }
            // otherwise it's a unkown token.
            else {
                free(lastToken);
                context->errorCode = 1; /* unkown token... */
                context->errorAtCharacterIndex = bufferLength - strlen(lastToken) + 1; // + 1 for the space
            }
            
        }
        // is this an op code?
        else if (isOpCode(bufferBeginPtr, bufferLength)) {
            *ignoreTheRestOfThisLine = true;
        }
        // if it's not an op code and this is the first token so far, we can assume it's a label name.
        else {
            // check if this token is one that already exists. If so, replace it with the
            // copy over the current token
            lastToken = malloc(sizeof(char) * bufferLength);
            strncpy(lastToken, bufferBeginPtr, bufferLength - 1);
            *emptyBufferByN = bufferLength;
        }
    }
    return NULL;
}

void allUpperCasePass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    if (*assembly >= 'a' && *assembly <= 'z') {
        *assembly = *assembly + ('A' - 'a');
    }
}

void removeNewLinePass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    if (*assembly == '\n') {
        *emptyBufferByN = 1;
    }
}

void removeExtraSpaces(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    char c = *assembly, nextC = *(assembly + 1);
    if (c == ' ' && (nextC == ' ' || nextC == '\n')) {
        *emptyBufferByN = 1;
    }
}

void removeCommentsPass(char *assembly, bool* addBufferToNewAsm, size_t* emptyBufferByN, size_t bufferLength, int* tmp) {
    enum CommentType { CommentTypeNo = 0, CommentTypeNewLine = 1, CommentTypeMultiLine = 2 };
    char c = *assembly, lastC = *(assembly - 1), nextC = *(assembly + 1);
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
        // flag for us to know that a comment is going on. (only set it if it hasn't been already)
        if (*tmp == CommentTypeNo) {
            *tmp = nextC == '*' ? CommentTypeMultiLine : CommentTypeNewLine;
        }
    }
}

// MARK: - Public
const size_t NUMBER_OF_PRE_PROCESSOR_OPS = 5;

void initPreProcessorContext(struct PreProcessorContext *context, size_t memSize) {
    context->errorCode = 0;
    context->errorAtCharacterIndex = 0;
    context->numberOfLabels = 0;
    context->numberOfValues = 0;
    context->memSize = memSize;
    context->labels = malloc(0 * sizeof(struct PreProcessorLabelDefinition));
    // temp storage for pre-processor operations
    context->tempPassStorage = malloc(sizeof(int) * NUMBER_OF_PRE_PROCESSOR_OPS);
}

void deinitPreProcessorContext(struct PreProcessorContext *context, void** memory) {
    free(context->tempPassStorage);
    // free all the labels.
    for (int i = 0; i < context->numberOfLabels; ++i) {
        struct PreProcessorLabelDefinition *label = context->labels[i];
        free(label->name);
        // value is optional, so check if it's set first.
        if (label->value) {
            free(label->value);
            free(label->type); // all values have types, so set free it here.
        }
        free(label);
    }
    free(context->labels);
}

bool prefixBeginsWithValidCharacter(char prefix) {
    // only start "pre-processing" until you find an alpha numeric character.
    return (prefix >= 'A' && prefix <= 'Z') || (prefix >= 'a' && prefix <= 'z') || (prefix >= '0' && prefix <= '9');
}

char* preProcessAssemblyCode(struct PreProcessorContext *context, size_t bufferSize, uintmax_t lineAddress, char* assemblyCode) {
    size_t asmCodeLen = strlen(assemblyCode);
    
    int bufferIndex = 0, asmIndex = 0, numberOfCompletedLoops = 0;
    char buffer[bufferSize], *assembly;
    do {
        // get a pointer to the current asm character.
        assembly = assemblyCode + asmIndex++;
        // ignore line certain prefix characters
        if (numberOfCompletedLoops == 0 && !prefixBeginsWithValidCharacter(*assembly)) {
            continue;
        }
        // pre-processor flags
        bool addBufferToNewAsm = true;
        size_t emptyBufferByN = 0;
        
        // run preprocessor passes:
        // the order here is based on priority. And it is extremely important. goes from least to most "important".
        size_t pass = 0;
        const size_t bufferLength = bufferIndex + 1;
        removeNewLinePass(assembly, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        removeExtraSpaces(assembly, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        allUpperCasePass(assembly, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        removeCommentsPass(assembly, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        struct PreProcessorLabelDefinition *newLabel = extractLabelsPass(context, assembly, &addBufferToNewAsm, &emptyBufferByN, bufferLength, &context->tempPassStorage[pass++]);
        
        // check the context for errors...
        if (context->errorCode > 0) {
            return NULL;
        }
        
        // check for new labels
        if (newLabel) {
            // validate label isn't a duplicate
            
            // finish setup of the new label
            newLabel->address = lineAddress;
            addLabelToContext(context, newLabel);
        }
        
        // build buffer
        char c = *assembly;
        buffer[bufferIndex++] = c;
        
        if (emptyBufferByN > 0) {
            // reset the index and length for future ops
            bufferIndex -= emptyBufferByN;
            char* bufferOffsetPtr = buffer + bufferIndex;
            memset(bufferOffsetPtr, 0, emptyBufferByN);
        }
        // increment loop counter.
        numberOfCompletedLoops++;
    }
    while (asmIndex < asmCodeLen);
    
    // check if we have something valid to return
    if (bufferIndex == 0) {
        return NULL;
    }
    // return the processed assembly code
    const size_t bufferLength = bufferIndex + 1;
    char *newAssembly = malloc(sizeof(char) * bufferLength);
    strncpy(newAssembly, buffer, bufferLength);
    return newAssembly;
}

/*!
 *  Pre-Processor Status Codes:
 *  ----------------------------------
 *  001: Use of unidentified Token: "Expected valid token."
 */
void retrieveInfoForPreProcessorStatusCode(uint8_t code, char** title, char** message) {
    switch (code) {
        case 001:
            *title = "Use of unidentified Token";
            *message = "Expected valid token.";
            break;
            
        default:
            *title = "Unknown";
            *message = "Unknown Error";
            break;
    }
}
