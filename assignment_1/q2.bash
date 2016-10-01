#!/bin/bash

if (( $# != 1 )); then
	echo "Please pass in only one argument."
	echo "Call this script with ${0} <number_of_photos>"
	exit
fi

while true; do
	for i in *.jpg; do
		number=`ps | grep eog | wc -l`
		while(($number>=$1)); do
			sleep 1
			number=`ps | grep eog | wc -l`
		done
		echo 'Opening viewer for' $i
		eog -n $i &
		echo 'There are' `ps | grep eog | wc -l` 'current viewers open'
	done
done