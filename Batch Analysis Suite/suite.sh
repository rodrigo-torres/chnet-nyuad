#!/bin/bash

directory=$(pwd)

function start {
	
	#Set initial files and variables
	updatelist
	tcount
		
	current
	autoscale
	updatecal
	
	#Source dependent plot.sh and fit.sh scripts
	source $directory/Script/plot.sh
	source $directory/Script/fit.sh
}

function updatelist {
	filelist=()
	for filename in $(find *Sample*Spectrum* -maxdepth 0 -type f -not -name "*.log" -not -name "*.plot" -not -name "*.png");
	do
		filelist+=($filename)
		echo "#!/usr/bin/gnuplot -persist" > ${filename}'.plot'
		echo -e "\nload '"${filename}".log'\nload 'Script/preamble'" >> ${filename}'.plot'
	done
	
	sed -i '/FileArray \=/d' $directory/Script/variables.dat
	echo "FileArray = \""${filelist[*]}"\"" >> $directory/Script/variables.dat
}

function current {
	echo -e "Choose an active file from the menu below:\n"
	select opt in "${filelist[@]}"; 
	do
		if [[ "${filelist[@]}" =~ "$opt"  ]]; then
			currentfilechoice=$opt
			autoscale
			echo -e "\nYou selected : "$currentfilechoice
			return
		else
			clear
			echo "Selection not valid. Try again."
		fi	
	done
}

function autoscale {
	#scalefile=$1
	uppercount=$(awk '{print $1}' $currentfilechoice | sort -nr | head -1)
	
	sed -i '/uppercount \=/d' $directory/Script/variables.dat	
	echo 'uppercount = '${uppercount} >> $directory/Script/variables.dat

}

function routine {
	opt=$(echo $1)
	
	case $opt in 
	1)
		doall KA
		doall CaA
		doall MnA
		doall FeA
		doall FeB
		;;
	*)
		echo -e "Routine specified does not exist"
		;;
	esac	
}

function updatecal {
	source $directory/Script/calibration.sh
	echo -e "Gradient set at: \t"${grad}" keV/Channel"
	echo -e "Offset set at: \t\t"${off}" keV"
}

function quit {
	exit 1
}

function mcount {
	echo -e "Which file should the program use?\n"
	for i in "${filelist[@]}";
	do
		echo $i
	done
	echo -e "\n"
	read filechoice
	
	echo -e "Which mode should be run, channel (type 0) or energy (type 1)?\n"
	read modechoice
	
	
	case $modechoice in 
	0)
		echo -e "Please input lower channel bound:\t"
		read lower
	
		echo -e "Please input upper channel bound:\t"
		read upper	
		
		awk -v lower=${lower} -v upper=${upper} '{
			if (NR >= lower && NR <= upper) print $1=$3}' $fitfile > tempman
		awk '{ sum += $1 } END { print sum }' tempman | xargs -I{} echo "# Counts between $lower and $upper {}" >> ${filechoice}'.log'
		rm tempman
		;;
	1)
		echo -e "Please input lower energy bound:\t"
		read lower
	
		echo -e "Please input upper energy bound:\t"
		read upper	
		
		awk -v lower=${lower} -v upper=${upper} -v m=${grad} -v c=${off} '{if (NR*m+c >= lower && NR*m+c <= upper) print $3}'  $filechoice > tempman
		awk '{ sum += $1 } END { print sum }' tempman | xargs -I{} echo "# Counts between $lower and $upper inclusive: {}" >> ${filechoice}'.log'
		rm tempman
		;;
	esac	
	
}

function fitparam {
	
	#Used to modify the default fitting parameters
	local elementl=$(echo $1)
	local parameter=$(echo $2)
	local value=$(echo $3)
	
	if [[ "${elementlist[@]}" =~ "$elementl" ]]; then
		if [[ $parameter = *"low"* ]]; then
			sed -i "/${elementl}.*low.*/s/.*/${elementl}_lowlimit\=${value}/" ${directory}/Script/fitvariables.dat
		elif [[ $parameter = *"high"* ]]; then
			sed -i "/${elementl}.*high.*/s/.*/${elementl}_highlimit\=${value}/" ${directory}/Script/fitvariables.dat
		elif [[ $parameter = "a" ]]; then
			sed -i "/${elementl}a/s/.*/${elementl}a=${value}/" ${directory}/Script/fitvariables.dat
		elif [[ $parameter = "b" ]]; then
			sed -i "/${elementl}b/s/.*/${elementl}b=${value}/" ${directory}/Script/fitvariables.dat
		elif [[ $parameter = "c" ]]; then
			sed -i "/${elementl}c/s/.*/${elementl}c=${value}/" ${directory}/Script/fitvariables.dat
		else
			echo "[!] Parameter not valid"
		fi	
	else
		echo "[!] Spectral line not found in array, please add it manually."
	fi
}

function print {
	local parameter=$(echo $1)
	grep -i "${parameter}" $directory/Script/fitvariables.dat
}

function remove {
	
	elementl=$(echo $1)
	fromfile=$(echo $2)
	
	if [[ "${filelist[@]}" =~ "$fromfile" ]]; then
			#Removes instructions to plot fit curve and label on plot file.
			sed -i "/replot ${elementl}(x)/ { N; d; }" ${fromfile}".plot"
			
			#Removes fit parameters from the associated log file
			sed -i "/${elementl}a \=/d" ${fromfile}".log"
			sed -i "/${elementl}b \=/d" ${fromfile}".log"
			sed -i "/${elementl}c \=/d" ${fromfile}".log"
			
			sed -i "/${elementl}_Counts \=/d" ${fromfile}".log"
			sed -i "/${elementl}_PCounts \=/d" ${fromfile}".log"
	else		
		for filename in "${filelist[@]}";
		do
			#Removes instructions to plot fit curve and label on plot file.
			sed -i "/replot ${elementl}(x)/ { N; d; }" ${filename}".plot"
			
			#Removes fit parameters from the associated log file
			sed -i "/${elementl}a \=/d" ${filename}".log"
			sed -i "/${elementl}b \=/d" ${filename}".log"
			sed -i "/${elementl}c \=/d" ${filename}".log"
			
			sed -i "/${elementl}_Counts \=/d" ${filename}".log"
			sed -i "/${elementl}_PCounts \=/d" ${filename}".log"
		done
	fi
}

function tcount {
	for filename in "${filelist[@]}"; do 
	echo -e "Filename = \"${filename}\"" > ${filename}'.log' 
	awk '{sum += $1} END {print sum}' $filename | xargs -I{} echo -e "Total_Counts = {}" >> ${filename}'.log'
	done
}

function doall {
	elementl=$(echo $1)
	fit $elementl
	count $elementl
	plot $elementl
}

# Accepts a parameter which specifies the spectral line to be quantified
function count {

	element=$(echo $1)
	elementlist=( "CaA" "CaB" "CoA" "CoB" "CrA" "CrB" "CuA" "CuB" "FeA" "FeB" "KA" "KB" "MnA" "MnB" "NiA" "NiB" "SA" "SB" "SiA" "SiB" "TiA" "TiB" )
	
	counter=1
	
	
	#Parses through the parameters available and exits the function if the argument doesn't match any of the available parameters
	for i in "${elementlist[@]}"
	do
		if [[ $element == $i ]]; then
			break
		elif [[ ${#elementlist[@]} == $counter ]]; then 
			echo "[!] Parameter specified not found"
			return
		fi
		counter=$[counter + 1]
	done
	
	source $directory/Script/calibration.sh
	
	for filename in "${filelist[@]}"; do
		#Gets the fitting parameters, only the last match per file
		
		element_a=$(grep  -in $element'a = ' $filename'.log' | tail -1 | awk '{print $3}')
		element_b=$(grep  -in $element'b = ' $filename'.log' | tail -1 | awk '{print $3}')
		element_c=$(grep  -in $element'c = ' $filename'.log' | tail -1 | awk '{print $3}')

		#Sets the parameters for counting the events
		lower=$(echo "${element_b}-2*${element_c#-}" | bc)
		upper=$(echo "${element_b}+2*${element_c#-}" | bc)
		
		echo $filename
		echo $lower $upper
		echo $grad $off
		
		#Parses the XRF spectrum file and prints out only the lines within some specified bounds, then sums the events in these lines
		awk -v lower=${lower} -v upper=${upper} -v m=${grad} -v c=${off} '{
		if (NR*m+c > lower && NR*m+c < upper) print $3}' $filename > ${filename}_${element}N;		
		element_counts=$(awk '{ sum += $1 } END { print sum }' ${filename}_${element}N)
		
		echo $element_counts
		Total_Counts=$(awk 'FNR == 2 {print $3}' $filename'.log')
		element_PCounts=$(bc -l <<< "scale=2;(${element_counts}*100/${Total_Counts})")
		
		
		
		#Appends the spectral line total and percentual counts to the logfile
		echo -e "${element}_Counts = ${element_counts}" >> $filename'.log'
		echo -e "${element}_PCounts = ${element_PCounts}" >> $filename'.log'
		
		#Deletes temporary files
		rm ${filename}'_'${element}'N'
	done
}



