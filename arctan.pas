{2008-06-04}
var
  k:int64;
  A,B,T,i:longint;
begin
  readln(T);
  for i:=1 to T do
  begin
    readln(A);
    k:=1 shl 62;
    for B:=A+1 to 2*A do
      if ((int64(A)*B+1)mod(B-A)=0) then
        k:=(int64(A)*B+1)div(B-A)+B;
    writeln(k);
  end;
end.
