CSS 448
Part 3
Hugo, Derek, Nina

-Design: As variables are generated, they are fed into a buffer which will then empty into the Symbol Table (which is generated using a stack and stackframes). As such, if a, b, and c are declared, they are put into a buffer which is then fed into the current stackframe. 

-Scope Handling: a Stack is used to maintain the Stackframes, which hold the variables and data.

-Who did what: Derek designed most of program, Hugo and Nina worked alongside him for implementation, with Hugo focusing more on things like pointers, and Nina forcusing on testing and debugging.
