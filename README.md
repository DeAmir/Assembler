# Assembler
An Assembler of a MIPS-like assembly language, written in C.\
Produces 3 files for each .as file given for processing:
* .obj - the memory image, which consists of memory addresses + values in these addresses (in octal form). The memory contains firstly the code segment of the .as (each instruction corresponds to an octal value) and then the data segment (e.g. .string "hello").
* .ent - labels marked for use out of the file. Consists of (labelName, labelAddress) pairs.
* .ext - labels marked as defined in some other file. Consists of (labelName, addressOfUse). External label may be used several times, in which case the pair would appear as many times as the no. of uses.

The idea is that a later process will link between the labels of different files via the .ext and .ent files, getting the values through the .obj file.

## How to run
To compile run _make_, then run _./assembler_ with assembly files as cmd args.\
For example: _./assembler valid_1_

## Examples of valid and invalid assembly code
Examples are given in valid_i.as files and in errors_1.as.
