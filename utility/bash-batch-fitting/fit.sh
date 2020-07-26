#!/bin/bash

directory=$(pwd)

function fit {
	elementl=$(echo $1)
	fromfile=$(echo $2)
	
	if [[ "${filelist[@]}" =~ "$fromfile" && "$fromfile" != "" ]]; 
	then
		echo "... Fitting ${fromfile}"
		local low=$(grep "${elementl}_lowlimit" Script/fitvariables.dat)
		local high=$(grep "${elementl}_highlimit" Script/fitvariables.dat)
		echo ${low: -4} ${high: -4}		
		gnuplot -e "
			load '$directory/Script/variables.dat'; 
			load '$directory/Script/fitvariables.dat';
			load '$directory/Script/functions.dat';
			low=${low: -4};
			high=${high: -4};
			fit [low:high] ${elementl}(x) '${fromfile}' u (\$0*m+c):1 via ${elementl}a,${elementl}b,${elementl}c; 
			set print '${fromfile}'.'.log' append; 
			print '${elementl}a = ',${elementl}a; 
			print '${elementl}b = ',${elementl}b; 
			print '${elementl}c = ',${elementl}c; 
			set print"
	rm fit.log
	
	else
		local low=$(grep "${elementl}_lowlimit" Script/fitvariables.dat)
		local high=$(grep "${elementl}_highlimit" Script/fitvariables.dat)
		echo ${low: -4} ${high: -4}	
		gnuplot -e "
			load '$directory/Script/variables.dat'; 
			load '$directory/Script/fitvariables.dat';
			load '$directory/Script/functions.dat'; 
			low=${low: -4};
			high=${high: -4};
			do for [i=1:words(FileArray)] {
				fit [low:high] ${elementl}(x) word(FileArray, i) u (\$0*m+c):1 via ${elementl}a,${elementl}b,${elementl}c; 
				set print word(FileArray, i).'.log' append; 
				print '${elementl}a = ',${elementl}a; 
				print '${elementl}b = ',${elementl}b; 
				print '${elementl}c = ',${elementl}c}; 
				set print"
			rm fit.log
	fi		
}


