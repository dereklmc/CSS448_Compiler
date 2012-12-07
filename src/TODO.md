# TODO List

1. GenerateCode() for arrays and set types
    - <s>_Hugo_Fix bugs showing up in sttest.p, where multidimensional array showing wonky dimensions </s> done?
	- <s>_Hugo_Fix non-typedef arrays in var declarations </s> done?
2. Handle calling Procedures
	- <s> _Nina_ Standard Procedure: write, writeln, read, readln in IOStatement within grammar </s>
	- <s> Standard Procedure: new, dispose in MemoryStatement within grammar. </s> I think this is done?
	- Non-standard Procedure Types
3. Handle calling Functions
	- Return type checking
<<<<<<< HEAD
	- Handle checking for returns by looking at function name
4. <s>_Derek_ Generate Code for subroutines<s>
5. <s>_Nina_ For/while loops, repeat statements</s> 
=======
4. ~~_Derek_ Generate Code for subroutines~~
5. ~~_Nina_ For/while loops, repeat statements~~
	- Still need to uncomment type checking when Constants are figured out
6. ~~_Nina_ Case Statements~~
7. _Derek_ Constants
	- Properly scope constant
	- Infer type?
    - Implement Symbol#isConstant bool
        - Defaults to false
    - Figure out how to generate Constants
        - Look at the Dragon Book
8. POINTERS
	-Designator stuff
	-General pointer usage stuff
	-Linked lists?
9. Designators 
	- Pointers (see 9)
	- array referencing      
10. Log errors
	- line #s
11. Print errors
12. Comments
13. _Derek_ Change Function#call to return itself, by reference.
