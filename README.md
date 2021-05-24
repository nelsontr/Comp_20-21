<a href="http://fenix.tecnico.ulisboa.pt"><img align="right" src="https://fenix.tecnico.ulisboa.pt/api/bennu-portal/configuration/logo" alt="Fenix Tecnico"></a>

# Comp_20-21

Compilers 2020/2021

**Group 019**

- @franciscosaiote
- @nelsontr

### Projects
- [x] [Zero](https://github.com/nelsontr/Comp_20-21/tree/project_zero)
- [x] [Intermediate](https://github.com/nelsontr/Comp_20-21/tree/project_intermediate)
- [ ] [Final]()

### Grades
| Solution          | Grade 				|
| :-----------------| :-------------------: |
| [Zero](https://github.com/nelsontr/Comp_20-21/tree/project_zero)			| *1.40* / 2.00           |
| [Intermediate](https://github.com/nelsontr/Comp_20-21/tree/project_intermediate)	| *5.13* / 6.00           |
| [Final]()         | *00.0* / 12.0           |


# CVS

[Mais Aqui](CVS.md)


# Avaliação

## Método de Avaliação do Projecto

A avaliação relativa à componente do Projecto processa-se em várias fases:

- Entrega Zero (inicial): projecto base (**obrigatória**)
- Entrega Intermédia: gerador de XML
- Entrega Final: compilador completo (**obrigatória**)
- Teste prático (**obrigatório**)

O Projecto (trabalho conducente às entregas acima mencionadas e abaixo  descritas) é realizado por grupos de, no máximo, 2 (dois) elementos,  durante o período estabelecido.

O Teste Prático é realizado individualmente, em data e local a agendar.

**A não realização de componentes obrigatórias tem como consequência a  classificação de zero valores na componente de projecto e exclusão da  avaliação em época normal.**

**TODAS AS ENTREGAS SÃO REALIZADAS ATÉ ÀS 17:00 DAS RESPECTIVAS DATAS**

## Cálculo da Nota do Projecto e Condições de Aprovação

Componentes de avaliação:

- **PRJ** - nota final do projecto
- **E0** - nota da entrega "zero"
- **EI** - nota da entrega intermédia
- **EF** - nota da entrega final
- **TP** - nota do teste prático

A nota do teste prático condiciona a distância ao mínimo entre as notas  do teste prático e a do projecto: desde um mínimo de 12.5% de acréscimo à menor das duas (abaixo de 7.5 valores no TP), até um máximo de 25%  (para 20 valores no TP). O acréscimo é linear entre 7.5 e 20. 

Ou seja:

- **PRJ = min(E0+EI+EF, TP) + | TP - E0 - EI - EF | \* max(12.5, TP+5) / 100**

**Condições necessárias para aprovação à disciplina (necessárias todas):**

- **PRJ >= 9.5** (sem arredondamento)
- **PRJ != NA**
- **TP > 0**
- **TP != NA**

# Entregas

## Entrega Zero (inicial) - Projecto Base

**Esta entrega é obrigatória e tem uma classificação máxima de 2 valores.****
**

- A Entrega "Zero" avalia o estado do projecto relativamente à concepção  dos nós que representam os conceitos da linguagem (sem ser necessário  tratar ainda do seu reconhecimento ou processamento).
- Não serão executados testes automáticos nesta entrega.
- Nesta fase, além da estrutura básica do compilador, todas as classes dos nós  da linguagem, assim como os esqueletos dos "visitors" (xml_writer,  postfix_writer, etc.) devem estar implementados. Não é ainda necessário  ter implementado nenhum código de análise lexical, sintáctica ou  semântica.
- Ver pormenores na pauta da avaliação da entrega "zero".
- **Considerando que é um passo crucial na concepção do projecto, a não realização desta entrega conduz automaticamente a uma classificação de 0 (zero) na  componente de avaliação relativa ao Projecto e consequente exclusão da  avaliação da disciplina no ano lectivo corrente.**

## Entrega Intermédia - Gerador de XML

**Esta entrega não é obrigatória e tem uma classificação máxima de 6 valores.**

- A Entrega Intermédia avalia o estado do projecto relativamente a um mínimo de funcionalidade.
- Não serão executados testes automáticos nesta entrega.
- Ver pormenores na pauta da avaliação da entrega intermédia.

## Entrega Final - Compilador Completo

**Esta entrega é obrigatória e tem uma classificação máxima de 12 valores.****
**

- A Entrega Final pressupõe que todo o projecto foi implementado.
- Serão executados testes automáticos nesta entrega.
- Os testes correspondem a uma série de programas que deverão ser compilados pelo resultado do projecto de cada grupo e cuja execução deve  corresponder a um conjunto de resultados padrão.
- **A não  realização da Entrega Final conduz automaticamente a uma classificação  de 0 (zero) na componente de avaliação relativa ao Projecto e  consequente reprovação à disciplina no ano lectivo corrente.**

## Teste Prático

- O Teste Prático consiste em fazer pequenas alterações/extensões ao  projecto descrito no enunciado. O teste é realizado com base na  implementação submetida para a avaliação correspondente à entrega final.
- O teste prático é como uma discussão de projecto, pelo que não existem repescagens como nos testes escritos. 
- Concretamente, o teste prático corresponde à resolução uma pergunta que aborda os  seguintes três tópicos: análise lexical; análise sintáctica; e análise  semântica e geração de código. Cada tópico pode ser resolvido com mais  ou menos profundidade (embora isso implique funcionamento possivelmente  deficiente a jusante) e será avaliado independentemente.
- **A não realização do Teste Prático conduz automaticamente a uma  classificação de 0 (zero) na componente de avaliação relativa ao  Projecto e consequente reprovação à disciplina no ano lectivo corrente.**
