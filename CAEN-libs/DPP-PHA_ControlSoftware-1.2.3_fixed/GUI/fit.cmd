labelmsg1 = "FIT MODE. Press 'f' to define a ROI and fit within it."
set label 1 labelmsg1 at graph 0.25,0.95
replot
bind 'f' load 'roi.cmd'
a = NaN
b = NaN
c = NaN