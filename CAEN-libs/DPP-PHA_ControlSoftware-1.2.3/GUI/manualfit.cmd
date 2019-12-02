
f(x) = a * exp((x-b)*(b-x)/(c**2)) 

set label 1 "Fit didn't converge. Press 'f' to start over or <Enter> to exit fit mode" at graph 0.25,0.95
replot
bind 'f' load 'roi.cmd'
bind "Return" load 'bail.cmd'

if (fa==0 && fb==0 && fc==0) call 'dofit.cmd' "via a,b,c";
if (fa==0 && fb==1 && fc==1) call 'dofit.cmd' "via a";
if (fa==1 && fb==0 && fc==1) call 'dofit.cmd' "via b";
if (fa==1 && fb==1 && fc==0) call 'dofit.cmd' "via c";
if (fa==0 && fb==0 && fc==1) call 'dofit.cmd' "via a,b";
if (fa==0 && fb==1 && fc==0) call 'dofit.cmd' "via a,c";
if (fa==1 && fb==0 && fc==0) call 'dofit.cmd' "via b,c";
if (fa==1 && fb==1 && fc==1) call 'dofit.cmd' "";

plot PlotData u 1:2 title 'data' w histeps 1, '' u 1:($1> xmin && $1<xmax ? $2 : 1/0) title 'ROI' w boxes 5 fill empty,  '' u 1:($1>xmin && $1<xmax ? f($1) : 1/0 ) title 'fit' w l 2

set label 1 "Curve fitted. Press 'f' to start over or <Enter> to exit fit mode" at graph 0.25,0.95
replot

print "y ",xmin," ",xmax," ",a," ",b," ",c
set print FitRes
print "y ",xmin," ",xmax," ",a," ",b," ",c
set print
