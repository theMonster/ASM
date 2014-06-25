//
//  asm.swift
//  lc3
//
//  Created by Caleb on 6/22/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

import Foundation

extension Int {
    func ToBinaryString() -> String {
        var binaryString = ""
        var numberCpy = self
        while (numberCpy != 0) {
            binaryString = String(numberCpy % 2) + binaryString
            numberCpy /= 2
        }
        return binaryString
    }
}

class ASM {
    var mem:Memory = Memory()
    init() {
        // validate instructions, memory requirements, general purpose registers, etc.
        var a = [mem[0], mem[0], mem[1]]
        mem[0].value = 2
        mem[1].value = 3
        add(&a)
        println(mem[0].value)
    }
}
