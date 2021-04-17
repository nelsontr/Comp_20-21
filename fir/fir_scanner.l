%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
FLOAT        ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?
LETTER       [A-Za-z_]
IDENTIFIER   {LETTER}({LETTER}|[[:digit:]])*

%x X_STRING X_COMMENT X_SPECIALCHAR
%%

  /* OPERATORS */

"sizeof"			         return tSIZEOF;

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 

"&&"                   return tAND;
"||"                   return tOR;

  /* SKIP OPERATORS */

"leave"			   	          return tLEAVE;
"restart"			            return tRESTART;
"return"			            return tRETURN;

  /* INPUT OUTPUT */

"@"                           return tINPUT;
"write"                       return tWRITE;
"writeln"                     return tWRITELN;  

  /* KEYWORDS */

  /*"void"                       return tVOID_TYPE;*/
"int"                         return tINT_TYPE;
"float"				          return tFLOAT_TYPE;
"string"			          return tSTRING_TYPE;
"null"                        return tNULLPTR;
[<>]                          return *yytext;

  /* CICLES */

"while"                       return tWHILE;
"do"                          return tDO;
"finally"                     return tFINALLY; 


   /*Símbolos globais*/
"public"                      return tPUBLIC;


  /* CONDITIONS */

"if"                          return tIF;
"then"                        return tTHEN; 
"else"                        return tELSE; 

  /* IDENTIFIERS */

{IDENTIFIER}  yylval.s = new std::string(yytext); return tIDENTIFIER;

  /* COMMENTS */

  /* MISSING SOME CASES - SEE WIKI EXERCICES*/
"!!".*\n                        ; // ignore comments 

"(*"                            yy_push_state(X_COMMENT);
<X_COMMENT>"(*"                 yy_push_state(X_COMMENT);
<X_COMMENT>"*)"                 yy_pop_state();
<X_COMMENT>.|\n                 ; /* ignore content */

  /* STRINGS */

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\~                yy_push_state(X_SPECIALCHAR);
<X_STRING>\'           {
                                yylval.s = new std::string(strlit.str());
                                strlit.str("");
                                yy_pop_state();
                                return tSTRING;
                              }
<X_STRING>.            strlit << *yytext;
<X_STRING>\n           yyerror("newline in string");


<X_SPECIALCHAR>n              { strlit << '\n'; yy_pop_state(); }
<X_SPECIALCHAR>r              { strlit << '\r'; yy_pop_state(); }
<X_SPECIALCHAR>t              { strlit << '\t'; yy_pop_state(); }
<X_SPECIALCHAR>\'           { strlit << "~'"; yy_pop_state(); }
<X_SPECIALCHAR>\~           { strlit << "~~"; yy_pop_state(); }
<X_SPECIALCHAR>[[:xdigit:]]{1,2} {
                                 strlit << (char)(unsigned char)std::stoul(yytext, NULL, 16);
                                 yy_pop_state();
                               }
<X_SPECIALCHAR>0[[:alnum:]]* {
                                 strlit << '\0';
                                 yy_pop_state();
                               }
<X_SPECIALCHAR>.			         { strlit << *yytext; yy_pop_state(); }

  /* INTEGERS */

[0-9]+                 yylval.i = strtol(yytext, NULL, 10); return tINTEGER;

  /* FLOATS */
{FLOAT}                { yylval.d = strtod(yytext, NULL); return tREAL; }
  /*(...)*/

  /* OTHERS */

[-()<>=+*/%;{}.]       return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
