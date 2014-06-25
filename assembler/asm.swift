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
    }
}
