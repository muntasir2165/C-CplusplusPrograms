#!/bin/bash
testcases=./tests/testcase[1-40].txt
outputs=./tests/output[1-40].txt
i=1
failure=0

while [[ $i -le 40 ]]; do
	./draw2 "./tests/testcase$i.txt"  > ./tests/gbg1 2>> ./tests/gbg2
	tail -n +3 ./tests/gbg1 > ./tests/tmp; cat ./tests/gbg2 >> ./tests/tmp
	rm -f ./tests/gbg1 ./tests/gbg2
	cat "./tests/output$i.txt" | tail -n +3  > ./tests/tmp2
	DIFF="$( diff -b ./tests/tmp ./tests/tmp2 )"
	if [ "$DIFF" != "" ]; then
		failure=1
		echo "testcase$i.txt:" >> ./tests/failureLog_o
		echo ''
		echo $(diff -b ./tests/tmp ./tests/tmp2) >> ./tests/failureLog_o
	fi
	i=$(( $i + 1 ))
done

if [[ $failure -eq 0 ]]; then
	echo "OUTPUT TESTING SUCCESS!!"
else
	echo "Output testing failed"
	echo "See failureLog for details (./tests/failureLog_o)"
fi

rm -f ./tests/tmp ./tests/tmp2
