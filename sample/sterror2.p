program sterror2(input, output);     (* program example with more st errors *)
const
   myreal = 4.5;

type 
   applePtr = ^apple;
   basketPtr = ^basket;
   basket = record
      fuji: applePtr;
      next: basketPtr
   end;
   theArray = array [2..8] of ^integer;
   anotherArray = array [5..9] of ^apple;
begin                                
   (* apple undefined indirectly *)
end.  
