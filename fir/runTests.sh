#!/bin/bash

NUM=0
SUC=0
FAIL=0
echo ">> make clean all..."
make clean all | grep !""
set -e

for x in tests-fir-daily-202103091601/*.fir; do
    NUM=$[$NUM+1]
    DIFF=${x/tests-fir-daily-202103091601\//tests-fir-daily-202103091601\/out\/}    

    printf "\033[01;36m%s \t-- " ${x/tests-fir-daily-202103091601\//}
    
    (./fir $x | grep !"") || :
    if [ -s ${x%.fir}.asm ]; then
	    yasm -felf32 ${x%.fir}.asm -o ${x%.fir}.o
		ld -melf_i386 -o ${x%.fir} ${x%.fir}.o -lrts -L ~/compiladores/root/usr/lib/
    fi

    if [ -s ${x%.fir} ]; then
		(./${x%.fir} > ${x%.fir}.out | grep !"") || :
	    if [ -s ${x%.fir}.out ]; then
	        diff -cB -w ${x%.fir}.out ${DIFF%.fir}.out > ${x%.fir}.diff
		    if [ -s ${x%.fir}.diff ]; then
		        echo -en "\033[01;31mFAIL: ${x%.fir}. See file ${x%.fir}.diff" "\n"
				FAIL=$[$FAIL+1];
		    else
				echo -en "\033[01;32mSUCCESS" "\n"
			    rm -f ${x%.fir}.diff
			    rm -f ${x%.fir}
				SUC=$[$SUC+1];
		    fi
		else
	        echo -en "\033[01;31mFAIL: ${x%.fir}. Erro on executing \n"
			FAIL=$[$FAIL+1];
	    fi
	else 
        echo -en "\033[01;31mFAIL: ${x%.fir}. Erro on compiling \n"
		FAIL=$[$FAIL+1];
    fi

    

done

echo -en "\033[01;32mSUCCESS: " $SUC "of " $NUM " test" "\n"
echo -en "\e[5m"
echo -en "\033[01;31mFAIL: " $FAIL "of " $NUM " test"
echo -en "\e[0m"
printf "\t\033[01;33m%d" $((100*$SUC/$NUM))
echo "%"

cd tests-fir-daily-202103091601/; rm -f *.o *.asm *.out; cd ..;

