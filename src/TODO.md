# TODO List

1. _Easiest_ Add Const declarations to ST
   a. Implement ConstantExpressions 
        - UnaryOperator ConstFactor
        - ConstFactor
        - ystring
        - ynil
2. Add Type declarations to ST
   a. Deal with Record Types
   b. Deal with Array Types
   c. Deal with Pointer Types
   d. Deal with Set Types
3. _2nd Easiest_ Add Variable declarations to ST
4. Deal with issues with invalidity and recursion (email zander)
5. TEST EVERYTHING
6. Comments

## Zander email:

Hi,

We discovered that the return type for functions could be invalid. Should we then
skip parsing the code inside a function?

If we need to skip parsing, we foresee this being a hard problem. The best idea 
we have is to, somehow, tell YACC to skip the Block nonterminal in the rule for 
FunctionDecl. Looking at the YACC and Bison documentation, it isn't obvious how 
we would skip a non-terminal.

Regards,
Derek, Hugo, Nina
