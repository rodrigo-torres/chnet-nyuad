bind 'f' ''
set label 1 "Click on the plot to place the cursors defining a Region of Interest." at graph 0.25,0.95
replot
pause mouse
c1 = MOUSE_X
set arrow 1 from c1,GPVAL_Y_MIN to c1,GPVAL_Y_MAX nohead
replot
pause mouse
c2 = MOUSE_X
set arrow 2 from c2,GPVAL_Y_MIN to c2,GPVAL_Y_MAX nohead
replot
if (c1 >= c2) xmax = c1; xmin = c2; else xmax = c2; xmin = c1;
xmax = floor(xmax)
xmin = ceil(xmin)
print 'xmax=',xmax
print 'xmin=',xmin
if (xmax-xmin < 4) unset arrow 1; unset arrow 2; set label 1 labelmsg at graph 0.25,0.95; replot; exit;

ymax = 0
xpeak = 0
grab(x,y)= (y>ymax) ? (ymax=y, xpeak=x) : 0
plot [xmin:xmax] PlotData using 1:(grab($1,$2)) title ''

a = ymax
b = xpeak
c = (xmax-xmin)/2
f(x) = a * exp((x-b)*(b-x)/(c**2)) 

set label 1 "Fit didn't converge. Press <Enter> to exit fit mode" at graph 0.25,0.95
replot
bind "Return" load 'bail.cmd'
unset arrow 1
unset arrow 2


fit [xmin:xmax] f(x) PlotData using 1:($2 > 0 ? $2 : 1/0):(sqrt($2)) via a,b,c 

plot PlotData u 1:2 title 'data' w histeps 1, '' u 1:($1> xmin && $1<xmax ? $2 : 1/0) title 'ROI' w boxes 5 fill empty,  '' u 1:($1>xmin && $1<xmax ? f($1) : 1/0 ) title 'fit' w l 2

set label 1 "Curve fitted. Press 'f' to start over or <Enter> to exit fit mode" at graph 0.25,0.95

replot
print "y ",xmin," ",xmax," ",a," ",b," ",c
set print FitRes
print "y ",xmin," ",xmax," ",a," ",b," ",c
set print

bind 'f' load 'roi.cmd'