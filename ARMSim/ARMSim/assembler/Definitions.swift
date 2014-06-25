//
//  Definitions.swift
//  lc3
//
//  Created by Caleb on 6/22/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

import Foundation

/* instruction keys */
let iName = "instruction name"
let iSurname = "instruction name"
let iAnatomy = "instruction anatomy"
let iFuncPtr = "instruction function pointer"

let opCode = "Operation Code"
let register = "Register"

/* isa info */
let bitCount = 16
let generalPurposeRegisters = 8
let memSize = 256

/* instructions */
//let functionPtrs = [add, multiply, loadRegister, and, jump, not, returnFromSubroutine]
let operations = [
    [iName : "Add", iSurname : "ADD",  iAnatomy : addOperation, iFuncPtr : 0],
    [iName : "Multiply", iSurname : "MUL",  iAnatomy : multiplyOperation, iFuncPtr : 1]
]


/* operations */
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
