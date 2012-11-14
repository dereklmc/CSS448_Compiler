program A(input, output);     (* program A *)
   var  x, y, z: integer;

   procedure B(var i: integer;            (* scopeEntry B *)
               var j: integer;
                   k: integer);
   begin
   end;                                   (* scopeExit B *)

   procedure C;                           (* scopeEntry C *)
   begin
   end;                                   (* scopeExit C *)

   procedure D(var i: integer;            (* scopeEntry D *)
               var j: integer;
                   k: integer);
      var  x, y: integer;

      procedure E;                           (* scopeEntry E *)
         var  i: integer;
      begin
      end;                                   (* scopeExit E *)
   
      procedure F;                           (* scopeEntry F *)
         var  x, j, k: integer;
      begin
      end;                                   (* scopeExit F *)

   begin
   end;                                   (* scopeExit D *)

   procedure G;                           (* scopeEntry G *)
   begin
   end;                                   (* scopeExit G *)
   
begin                                
end.                                   (* scopeExit example *)

