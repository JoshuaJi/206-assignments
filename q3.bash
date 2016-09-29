#!/bin/bash

FILE_LIST=`ls *.dat`

case $1 in
	"alpha" )
		for FILE_NAME in $FILE_LIST; do
			TEMP_ARRAY=(${FILE_NAME//./})
			echo "${TEMP_ARRAY[@]}"
		done;;

	"weight" )
		;;

	"length" )
		;;
esac
