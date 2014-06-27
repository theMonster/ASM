//
//  definitions.h
//  ARMSim
//
//  Created by Caleb on 6/25/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

//#define iName "instruction name"
//#define iSurname "instruction surname"
//#define iAnatomy "instruction anatomy"
//#define iFuncPtr "instruction function pointer"
//
//#define opCode "Operation Code"
//#define register "Register"

#include <string.h>

enum {
    VOID_BIT,
    OPERATION_BIT,
    SOURCE_REGISTER_BIT,
    DESTINATION_REGISTER_BIT
};

#define BIT_COUNT 16
#define OP_CODE_LENGTH 4
#define MAX_OP_CODES 2^OP_CODE_LENGTH
#define GENERAL_PURPOSE_REGISTERS 8
#define memorySize 256

struct Operation {
    char* name;
    char* asmName;
    long opcode;
    long instructionAnatomy[BIT_COUNT - OP_CODE_LENGTH];
};



struct Operation getAddOperation() {
    struct Operation addOperation;
    addOperation.name = "Add";
    addOperation.asmName = "ADD";
    addOperation.instructionAnatomy[0] = DESTINATION_REGISTER_BIT;
    addOperation.instructionAnatomy[1] = DESTINATION_REGISTER_BIT;
    addOperation.instructionAnatomy[2] = DESTINATION_REGISTER_BIT;
    addOperation.instructionAnatomy[3] = SOURCE_REGISTER_BIT;
    addOperation.instructionAnatomy[4] = SOURCE_REGISTER_BIT;
    addOperation.instructionAnatomy[5] = SOURCE_REGISTER_BIT;
    addOperation.instructionAnatomy[6] = VOID_BIT;
    addOperation.instructionAnatomy[7] = VOID_BIT;
    addOperation.instructionAnatomy[8] = VOID_BIT;
    addOperation.instructionAnatomy[9] = SOURCE_REGISTER_BIT;
    addOperation.instructionAnatomy[10] = SOURCE_REGISTER_BIT;
    addOperation.instructionAnatomy[11] = SOURCE_REGISTER_BIT;
    return addOperation;
}

void add() {

}

/*
let iName = "instruction name"
let iSurname = "instruction name"
let iAnatomy = "instruction anatomy"
let iFuncPtr = "instruction function pointer"

let opCode = "Operation Code"
let register = "Register"

let bitCount = 16
let generalPurposeRegisters = 8
let memSize = 256

//let functionPtrs = [add, multiply, loadRegister, and, jump, not, returnFromSubroutine]
let operations = [
                  [iName : "Add", iSurname : "ADD",  iAnatomy : addOperation, iFuncPtr : ""],
                  [iName : "Multiply", iSurname : "MUL",  iAnatomy : multiplyOperation, iFuncPtr : ]
                  ]


// (all operations must have the same parameters) "inout a:Array<Register>"

let addOperation = [4:opCode, 3:register, 3:register, 3:000, 3:register]
func add(inout a:Array<Register!>) {
    var dr = a[0], sr1 = a[1], sr2 = a[2]
    dr.value = Int(sr1.value as Int) + Int(sr2.value as Int)
}

let multiplyOperation = [4:opCode, 3:register, 3:register, 3:000, 3:register]
func multiply(inout a:Array<Register!>) {

}

func loadRegister(inout a:Array<Register!>) {

}

func and(inout a:Array<Register!>) {

}

func jump(inout a:Array<Register!>) {

}

func not(inout a:Array<Register!>) {
    
}

func returnFromSubroutine(inout a:Array<Register!>) {
    
}
*/



