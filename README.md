ASM
===========

###INTRO
This project is a project to give assembly writers and those wishing to learn to write assembly have a simple and fun assembler to test against. Moreover, this project will allow for "*fexlible*" and modify-able instruction sets (ISA). This assembler can dynamically take an Instruction set, it can process the assembly code according to that ISA.

I've written part of the [LC-3](https://en.wikipedia.org/wiki/LC3) (with a couple of my own modifications / additional instructions) Instuction Set as a test target. Following the example code in the `lc3.h` and `lc3.c` files, you can create your own or modify an existing Instruction Set. 

I should note that even though I call this an Assembler, this actually is much closer to a virtual machine than a assembler since this doesn't (yet) go from ASM to machine code. In the future I'd like to be able to give the user the choice of translating it to either LLVM IR or to interpret it (like a VM).

###LC-3
In large part, the LC-3 is the motivation behind this project. I chose the LC-3 since it is one of the most popular ISA's for education environments. Unfortunatly, the original implementation of the LC-3 is largely outdated, very un-customizable and windows only. While learning the LC-3's ISA, I always wished that I could write a little C code to expose something so that I could grow the assigment into something much cooler. So being able to add

###Lean and mean
As you can tell by the number of files in the repo, this project is lean (<1000 lines of code). And I intend on keeping it that way. Other Assemblers are thousands upon ten's of thousands of lines of code. I don't believe in writing that much code unless it's completely necessary. When reading and observing these other assemblers, I see a lot of redundant code and complex approaches that can be simplified. So, while maintaining and writing for this project, I intend on keeping a firm hand with code reduction and simplicity. So that if someone wants to fork this repo, they can do so without spending hours just understanding a few lines of code. Or trying to muddle through all the source files figuring out which one goes where.

###Incomplete
This is still a work in progress and some features are still being implemented. Such as:

- Labels
- Command line friendly interface
- some sort of GUI interface
- LC-3 complete implementation
- PC (& other) registers exposed to the general purpose registers
- General Purpose memory
- LLVM IR support & easy export from LLVM IR to an executable binary.

I'm still thinking of new cool ideas for this assembler. If you have any, open up an issue and label it with "feature request".
