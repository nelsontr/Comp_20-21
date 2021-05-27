#!/bin/bash

NUM=0
SUC=0
FAIL=0
PASTA_TEST=tests-fir-daily-202103091601/

echo ">> make clean all..."
#make clean all | grep !""
#set -e

function fail {
	echo -en "\033[01;31mFAIL: ${x%.fir}.\n"
	FAIL=$[$FAIL+1];
}

function sucess {
	echo -en "\033[01;32mSUCCESS" "\n"
	rm -f ${x%.fir} ${x%.fir}.diff ${x%.fir}.out ${x%.fir}.o ${x%.fir}.asm
	SUC=$[$SUC+1];	
}

for x in $PASTA_TEST*.fir; do
    NUM=$[$NUM+1]
    DIFF=${x/$PASTA_TEST/$PASTA_TEST\/out\/}    

    printf "\033[01;36m%s \t|| " ${x/$PASTA_TEST/}
    
    (./fir $x | grep !"") || :
    if [ -s ${x%.fir}.asm ]; then
	    yasm -felf32 ${x%.fir}.asm -o ${x%.fir}.o
		ld -melf_i386 -o ${x%.fir} ${x%.fir}.o -lrts -L ~/compiladores/root/usr/lib/
    else
    	echo "ERROR"
    fi

    if [ -s ${x%.fir} ]; then

		(./${x%.fir} > ${x%.fir}.out | grep !"") || :
	    
	    if [ -s ${x%.fir}.out ]; then
	        
	        diff -cB -w ${x%.fir}.out ${DIFF%.fir}.out > ${x%.fir}.diff

		    if [ -s ${x%.fir}.diff ]; then
		        fail
		    else
				sucess
		    fi
		else
	        fail
	    fi
	else 
        fail
    fi

done

echo -en "\033[01;32mSUCCESS: " $SUC "of " $NUM " test" "\n"
echo -en "\e[5m"
echo -en "\033[01;31mFAIL: " $FAIL "of " $NUM " test"
echo -en "\e[0m"
printf "\t\033[01;33m%d" $((100*$SUC/$NUM))
echo "%"

make clean | grep !""
cd $PASTA_TEST; rm -f *.o *.asm *.out *.diff; cd ..;

