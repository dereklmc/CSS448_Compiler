CSS 448
Pascal Compiler
authors: Evelina Arthursson Derek McLean, Hugo Ribeiro

Section 1
===============================================================================
 Regular Pascal Programs
-------------------------------------------------------------------------------
----GOOD-----------------------------------------------------------------------

simplest.p      OK, G++ Compiles, Runs
simplest2.p     OK, G++ Compiles, Runs
if.p            OK, G++ Compiles, Runs
case.p          OK, G++ Compiles, Runs
subscripts.p    OK, G++ Compiles, Runs
pointer.p       OK, G++ Compiles, Runs
add.p           OK, G++ Compiles, Runs

Section 2
===============================================================================
===============================================================================
 Medium Pascal Programs
-------------------------------------------------------------------------------
----GOOD-----------------------------------------------------------------------

array2.p        OK, G++ Compiles, Runs
array.p         OK, G++ Compiles, Runs
list.p          OK, G++ Compiles, Runs

----ERROR----------------------------------------------------------------------

fact.p          - Error falsely being thrown when a constant (int) is being passed
				  into a function that has an integer for a parameter
				- problem at line 25, see above; missing line terminator in some error message.

Section 3
===============================================================================
===============================================================================
 Hard Pascal Programs
-------------------------------------------------------------------------------
----GOOD-----------------------------------------------------------------------

nestedprocs.p   OK, G++ Compiles, Runs
sttest3.p       OK, G++ Compiles, Runs
list2.p         OK, G++ Compiles, Runs
sttest.p        OK
sttest2.p       OK

----ERROR----------------------------------------------------------------------

set.p           NOT IMPLEMENTED


Section 4
===============================================================================
===============================================================================
 Erroneous Pascal Programs
-------------------------------------------------------------------------------
----GOOD-----------------------------------------------------------------------

simpleerror.p       OK
sterror2.p          
harderror.p         
funcerror.p         
arrayerror.p        
errors.p            
paramtypeerror.p    

----ERROR----------------------------------------------------------------------

sterror.p           - undefined type error prints twice
                    - identifiers are case sensitive.

===============================================================================

Other Programs
----GOOD----------------------------------------------------------------------
exp.p           OK, (working as expected)
----ERRORS--------------------------------------------------------------------
------------------------------------------------------------------------------


Section 5
-------------------------------------------------------------------------------
We did some pretty elaborate Type checking, in a variety of instances. Aside from
the basic type checking (mod and div operators, for example), we set up a
possibly over-extensive series of type checking. There weren't that many specific
errors that we were instructed to catch, but we did type checking with designators
and pointers, return types and parameters.

Section 6
-------------------------------------------------------------------------------
We went through lots of revisions throughout this phase, and thus our code may
have become a bit disorganized/confusing. 
For types, we have an interesting hierarchy of classes. We have the normal Type
object which spawns arrayType, stdType, recordType, etc. But also have two other
classes, TypeSymbol and SymbolicType that help represent anonymous Types, typedefs,
etc. 
For nested procedures, we generated code that made each procedure its own class, with
a public .call() method that actually performed the procedure code. For functions we
used this model but also incorporated handling for returns.
Constants are global, not scoped.
Don't handle passing arrays by value.

Section 7
-------------------------------------------------------------------------------
In class.
