#!/bin/bash
make
for i in 'postfix_writer' 'xml_writer' 'type_checker'
do
   echo $i
   cdk  target --language fir --target $i
   echo '----'
done
make clean