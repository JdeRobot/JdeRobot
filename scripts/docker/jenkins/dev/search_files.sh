#!/bin/bash


FILE_NOT_FONUD=64

for f in $1/*; do
	#echo "$f"
	while IFS='' read -r line || [[ -n "$line" ]]; do
	    if [ ! -f "$line" ] && [ ! -d "$line" ]; then
	    	echo "$line : File Not Found!" 1>&2
			exit $FILE_NOT_FONUD
		fi
		#echo "$line"
	done < "$f"
done