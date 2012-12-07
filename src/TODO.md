# TODO List

1. _Hugo_ GenerateCode() for arrays and set types
    - Fix bugs showing up in sttest.p, where multidimensional array showing wonky dimensions
	- Fix non-typedef arrays in var declarations 
2. <s> _Nina_Handle calling Procedures </s>
	- <s> _Nina_ Standard Procedure: write, writeln, read, readln in IOStatement within grammar </s>
	- <s> Standard Procedure: new, dispose in MemoryStatement within grammar. </s> I think this is done?
	- <s>Non-standard Procedure Types </s>
3. <s> _Nina_ Handle calling Functions </s>
	-  <s> Return type checking </s>
	- <s>Handle checking for returns by looking at function name </s>
4. <s>_Derek_ Generate Code for subroutines</s>
5. <s>_Nina_ For/while loops, repeat statements</s> 
		- Still need to uncomment type checking when Constants are figured out
6. <s>_Nina_ Case Statements </s>
7. _Derek_ Constants
	- Properly scope constant
	- <s>Infer type? </s> Nina: I think this is done, but you might need to check
    - Implement Symbol#isConstant bool
        - Defaults to false
8. POINTERS
	-Designator stuff
	-General pointer usage stuff
	-Linked lists?
9. Designators 
	- Pointers (see 9)
	- array referencing      
10. _Nina_Log errors
	- line #s
	- Halfway done, almost finished
11. Print errors
12. Comments
13. Take code out of grammar.y and incorporate it into methods in actions.cpp
14. Sort actions.cpp
