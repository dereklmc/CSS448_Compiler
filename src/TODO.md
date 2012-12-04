# TODO List

1. GenerateCode() for arrays and set types
    - Fix bugs showing up in sttest.p, where multidimensional array showing wonky dimensions
	- Fix non-typedef arrays in var declarations 
2. Handle calling Procedures
	- <s> _Nina_ Standard Procedure: write, writeln, read, readln in IOStatement within grammar </s>
	- Standard Procedure: new, dispose in MemoryStatement within grammar
	- Non-standard Procedure Types
3. Handle calling Functions
	- Return type checking
4. GenerateCode() for Procedure and Function calls
5. Create a PascalArray (template class - params type of array, and type of indices)
	- Defined in header file
	- Has an index converter (0 based)
	- Used for code generation
	- Recursively generate for multi-dimensional arrays
	(SEE NOTES IN 1)
6. <s> _Nina_ For/while loops, repeat statements </s> 
	- Still need to uncomment type checking when Constants are figured out
7. <s> _Nina_ Case Statements </s>
8. _Derek_ Constants
	- Properly scope constant
	- Infer type?
    - Implement Symbol#isConstant bool
        - Defaults to false
    - Figure out how to generate Constants
        - Look at the Dragon Book
9. POINTERS
	-Designator stuff
	-General pointer usage stuff
	-Linked lists?
10. Designators 
	- Pointers (see 9)
	- array referencing      
11. Log errors
	- line #s
12. Print errors
13. Comments
