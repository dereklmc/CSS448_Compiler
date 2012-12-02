# TODO List

1. GenerateCode() for arrays and set types
    - 
2. Handle calling Procedures
	- Standard Procedure: write, writeln, read, readln in IOStatement within grammar
	- Standard Procedure: new, dispose in MemoryStatement within grammar
	- Non-standard Proecure Types
3. Handle calling Functions
	- Return type checking
4. GenerateCode() for Procedure and Function calls
5. Create a PascalArray (template class - params type of array, and type of indices)
	- Defined in header file
	- Has an index converter (0 based)
	- Used for code generation
	- Recursively generate for multi-dimensional arrays
6. _Nina_ For/while loops, repeat statements
7. Case Statements
9. _Derek_ Constants
	- Properly scope constant
	- Infer type?
    - Implement Symbol#isConstant bool
        - Defaults to false
    - Figure out how to generate Constants
        - Look at the Dragon Book
        
10. Log errors
	- line #s
11. Print errors
12. Comments
