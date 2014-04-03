# Assembler

## Procedure

The assembler for SkagOS works in the following way:

1. It reads in a file name
2. It opens that file
3. It reads the file into an ASMSource object
    1. While reading, it strips comments and blank lines
4. It converts the ASMSource object into an ObjectSource object
    1. Line by line, each assembly function is converted to an ObjectSource code
5. It returns the ObjectSource object for use by the VirtualMachine

## Objects

- `Assembler`: The core of the assembler. It runs the above-listed procedure
- `Preprocessor`: Reads in a file name and returns a stripped ASMSource
- `ASMSource`: The source code of an assembly file
- `ObjectSource`: The converted object code of an assembly program
