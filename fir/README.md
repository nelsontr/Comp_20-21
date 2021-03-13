# Project Zero

## Warnings

In the final, before compiling everything (make) we need to make the instruction:
```bash
#!/bin/bash
make
for i in 'postfix_writer' 'xml_writer' 'type_checker'
do
   echo $i
   cdk  target --language fir --target $i
   echo '----'
done
make clean
```
(...) for every file = [postfix_write, xml_write, type_checker]

## Functions to define

* [x] **return (return_node.h)**

    **Q:** Colocamos expression_node, mas não podiamos ser mais especificos: colocar literal_node seria mais correto?
    
    **A:** (...)

    **Q:** O return pode ser null, ou ser um inteiro. Segundo a nossa gramatica dá a entender que é null mas no paragrafo a respeito do mesmo, dá a entender o contrario e ficamos confisos.
    
    **A:** (...)


* [x] **bloco (bloco_node.h)**

* [x] **if** *expression* **then** *instruction* [ **else** *instrução* ]

* [x] **while** *expression* **do** instruction [ **finally** *instruction* ]

* [x] **leave** [ *integer-literal* ] //break

  **Q:** Colocamos basic_node, mas estavamos na duvidas se não poderia ser um literal node, uma vez ques só pode ser um inteiro. Esta duvida surgiu porque não sabemos se o literal tem esta componente de poder não existir, i.e, de ser null.

  **A:** (...)

* [x] **restart** [ *integer-literal* ]

* [ ] **write** *expressões* ; | **writeln** *expressões* ;




