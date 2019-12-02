if (a == NaN) exit;
if (b == NaN) exit;
if (c == NaN) exit;
unset label 1
replot
set print FitRes
print xmin," ",xmax," ",a," ",b," ",c
set print
bind 'f' ''
bind "Return" ''