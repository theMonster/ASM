//
//  Definitions.swift
//  lc3
//
//  Created by Caleb on 6/22/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

import Foundation

/*#defines*/
let iName = "instruction name"
let iSurname = "instruction name"
let iAnatomy = "instruction anatomy"

let opCode = "Operation Code"
let register = "Register"

let bitCount = 16
let generalPurposeRegisters = 8
let memSize = 256
let operations = [
    [iName : "Add", iSurname : "ADD",  iAnatomy : addOperation]
]


/* operations */
let addOperation = [4:opCode, 3:register, 3:register, 3:000, 3:register]
func add() {

}

func loadRegister() {

}

func and() {

}

func jump() {

}

func not() {

}

func returnFromSubroutine() {

}
