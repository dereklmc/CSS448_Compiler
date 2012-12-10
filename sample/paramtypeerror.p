program paramtypeerror(input,output);

type
   array1D = array [1..10] of integer;

var
   list: array1D;
   number: integer;
   ch: char;
   found: boolean;

(**************************  procedure dosomething  **************************)

procedure dosomething(var list: array1D; size: integer);
begin
   write(size);
end;

(***************************  function junk  *********************************)

function junk(x: boolean; a: char): integer;
begin
   junk := "hello world";
end;

(********************************  main  *************************************)

begin
   dosomething(ch, list);             (* parameter types not correct *)
   number := junk(list, found);       (* parameter types not correct *)
end.

