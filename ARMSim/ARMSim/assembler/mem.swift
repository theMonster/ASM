//
//  mem.swift
//  lc3
//
//  Created by Caleb on 6/22/14.
//  Copyright (c) 2014 theMonster. All rights reserved.
//

import Foundation

class Register {
    var address:Double
    var value:AnyObject! = nil
    init(address:Double) {
        self.address = address
    }
}

class Memory {
    var gpr:Register[] // general purpose registers
    
    init() {
        // allocate the gpr
        gpr = []
        for i in 0..memSize {
            gpr += Register(address: Double(i))
        }
    }
    
    subscript(index:Int) -> Register! {
        return gpr[index]
    }
}
