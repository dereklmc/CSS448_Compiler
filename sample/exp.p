
program grade(input, output);

(*** Demonstrate the if.  ***)

var
   foo: integer;
   bar: integer;
   fizz: integer;
   buzz: integer;

(* **************************    main      ********************************** *)

begin
  foo := 2 * 3 + (4 + 5) / 3;
  bar := foo * 2 mod 5;
  fizz := (foo + bar) mod bar * foo;
  buzz := fizz + 6;
end.

