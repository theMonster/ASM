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

extern char* preProcessAssemblyCode(char* assemblyCode, size_t memSize, void** memory);

#endif /* preprocessor_h */
