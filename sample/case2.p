
program caseRepeat(input, output);

(*** Demonstrate the repeat and case statements.  ***)

var
   number, k, j, b: integer;
   p : char;

begin
  repeat 
     writeln('Enter year number, between 1 and 5, negative number to finish:');
     read(number);
     if (number >= 1) and (number <= 5) then begin
	   case number of
	     x: writeln("First year student");
	     k: writeln("Sophomore");
	     j: writeln("Junior");
	     p: writeln("Senior");
	     b: begin
		      writeln("Congratulations, you have graduated ... ");
		      writeln("Graduate student???");
            end
	   end
	 end
  until number < 0
end.

