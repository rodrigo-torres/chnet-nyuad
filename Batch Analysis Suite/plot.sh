#!/bin/bash

function plot {
	elementl=$(echo $1)
	
	if [[ $elementl = "gen" ]]; then
		case=0	
	elif [[ $elementl = "CaA" ]]; then
		case=1
	elif [[ $elementl = "CaB" ]]; then
		case=2
	elif [[ $elementl = "CoA" ]]; then
		case=3
	elif [[ $elementl = "CoB" ]]; then
		case=4
	elif [[ $elementl = "MnA" ]]; then
		case=5
	elif [[ $elementl = "FeA" ]]; then
		case=6
	elif [[ $elementl = "FeB" ]]; then
		case=7
	elif [[ $elementl = "KA" ]]; then
		case=8
	else
		echo "[!] Not a valid parameter"
		return 1
	fi	
	
	case $case in
	0)
		echo "Plotting custom functions is not implemented (yet)"
		return
		;;
	1)
		echo "replot CaA(x) title sprintf('Ca_{K{/Symbol a}} %0.1f',CaA_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 201 'Ca_{K{/Symbol a}}' rotate left at CaAb,CaAa+100" >> $currentfilechoice'.plot'
		;;
	2)
		echo "replot CaB(x) title sprintf('Ca_{K{/Symbol b}} %0.1f',CaB_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 202 'Ca_{K{/Symbol b}}' rotate left at CaBb,CaBa+100" >> $currentfilechoice'.plot'
		;;
	3)
		echo "replot CoA(x) title sprintf('Co_{K{/Symbol b}} %0.1f',CoA_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 271 'Co_{K{/Symbol a}}' rotate left at CoAb,CoAa+100" >> $currentfilechoice'.plot'
		;;
	4)
		echo "replot CoB(x) title sprintf('Co_{K{/Symbol b}} %0.1f',CoB_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 272 'Co_{K{/Symbol b}}' rotate left at CoBb,CoBa+100" >> $currentfilechoice'.plot'
		;;
	5)
		echo "replot MnA(x) title sprintf('Mn_{K{/Symbol a}} %0.1f',MnA_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 272 'Mn_{K{/Symbol a}}' rotate left at MnAb,MnAa+100" >> $currentfilechoice'.plot'
		;;
	6)
		echo "replot FeA(x) title sprintf('Fe_{K{/Symbol a}} %0.1f',FeA_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 261 'Fe_{K{/Symbol a}}' rotate left at FeAb,FeAa+100" >> $currentfilechoice'.plot'
		;;
	7)
		echo "replot FeB(x) title sprintf('Fe_{K{/Symbol b}} %0.1f',FeB_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 262 'Fe_{K{/Symbol b}}' rotate left at FeBb,FeBa+100" >> $currentfilechoice'.plot'
		;;
	8)
		echo "replot KA(x) title sprintf('K_{K{/Symbol a}} %0.1f',KA_PCounts) lt rgb '#000000'" >> $currentfilechoice'.plot'
		echo "set label 191 'K_{K{/Symbol a}}' rotate left at KAb,KAa+100" >> $currentfilechoice'.plot'
		;;
	esac
}



