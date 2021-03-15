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
Re-Making fir

```bash
make clean; make;
```


## Functions to define

* [x] **return (return_node.h)**

    **Q:** Colocamos expression_node, mas não podiamos ser mais especificos: colocar literal_node seria mais correto?
    
    **A:** (...)

    **Q:** O return pode ser null, ou ser um inteiro. Segundo a nossa gramatica dá a entender que é null mas no paragrafo a respeito do mesmo, dá a entender o contrario e ficamos confisos.
    
    **A:** (...)


* [x] **if** *expression* **then** *instruction* [ **else** *instrução* ]  (**if_block.h | if_else_block.h**)

  **OBSERVAÇÃO:** Colocamos 2 .h's, mas isso foi por causa do ano passado. Acho que só necessitamos de um, sendo que será o if_else onde o else terá nullptr caso não seja especificado

  **Q:** Fazer if_node ou if_else_node com o *_elseblock* predefinico com nullptr não seria a mesma coisa?

  **A:** (...)
  
* [x] **leave** [ *integer-literal* ] //break

  **Q:** Colocamos basic_node, mas estavamos na duvidas se não poderia ser um literal node, uma vez ques só pode ser um inteiro. Esta duvida surgiu porque não sabemos se o literal tem esta componente de poder não existir, i.e, de ser null.

  **A:** (...)
  
* [x] **bloco (bloco_node.h)**

* [x] **while** *expression* **do** instruction [ **finally** *instruction* ]

* [x] **restart** [ *integer-literal* ]

* [x] **write** *expressões* ; | **writeln** *expressões* ;

  **Q:** No Og tem os argumentos como sequence node, mas no print_node que vinha por predefinição vem como expression_node. Qual é que é a diferença?

  **A:** (...)

* [x] **Input** (read_node.h) ?

* [x] **Sizeof** - sizeof_node.h

* [x] **Left-values**

  Copiado - Explicar melhor

* [ ] **Variáveis** - variables_declaration_node.h

* [ ] **Identificadores** - index_node.h

* [ ] **Parênteses**

* [ ] **Pointers**

* [ ] **Identidade e simetrico**

* [ ] **Memoria** - address_of_node.h

* [ ] **Endereços**

