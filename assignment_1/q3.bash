#!/bin/bash

FILE_LIST_DAT=($(ls *.dat))
FILE_LIST_JPG=($(ls *.jpg))

if [[ $# != 1 ]]; then
	echo "please enter one argument [alpha], [weight], [length]"
	exit
fi

if [ ! -d 'result' ]; then
	mkdir result	
fi

case $1 in
	"alpha" )
		for FILE_NAME in ${FILE_LIST_DAT[@]}; do
			echo ${FILE_NAME%%.*}
		done
		convert *.jpg -append result/result.jpg
		eog result/result.jpg
		;;

	"weight" )
		WEIGHTS=($(cat *.dat|cut -d' ' -f1))

		SORTED_WEIGHTS=($(cat *.dat | cut -d' ' -f1 | sort -n))

		ORDER=( )
		ORDERED_FILES=( )

		for SORTED_WEIGHT in ${SORTED_WEIGHTS[@]}; do
			for (( i=0 ; i<${#WEIGHTS[@]} ; i++)); do
				if [ "${SORTED_WEIGHT}" = "${WEIGHTS[$i]}" ]; then
					ORDER+=($i)
				fi
			done
		done
		for INDEX in ${ORDER[@]}; do
			ORDERED_FILES+=(${FILE_LIST_JPG[$INDEX]})
			echo ${FILE_LIST_DAT[$INDEX]%%.*}
		done
		convert ${ORDERED_FILES[@]} -append result/result.jpg
		eog result/result.jpg
		;;

	"length" )
		LENGTHS=($(cat *.dat|cut -d' ' -f2))
		SORTED_LENGTHS=($(cat *.dat | cut -d' ' -f2 | sort -n))

		ORDER=( )
		ORDERED_FILES=( )

		for SORTED_LENGTH in ${SORTED_LENGTHS[@]}; do
			for (( i=0 ; i<${#LENGTHS[@]} ; i++)); do
				if [ "${SORTED_LENGTH}" = "${LENGTHS[$i]}" ]; then
					ORDER+=($i)
				fi
			done
		done
		for INDEX in ${ORDER[@]}; do
			ORDERED_FILES+=(${FILE_LIST_JPG[$INDEX]})
			echo ${FILE_LIST_DAT[$INDEX]%%.*}
		done
		convert ${ORDERED_FILES[@]} -append result/result.jpg
                eog result/result.jpg

		;;
esac
