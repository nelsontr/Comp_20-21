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
%x X_STRING X_COMMENT
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

"@"                    return tINPUT;
"write"                return tWRITE;
"writeln"              return tWRITELN;  

  /* KEYWORDS */

  /*"void"                       return tVOID_TYPE;*/
"int"                         return tINT_TYPE;
"float"				                return tFLOAT_TYPE;
"string"			                return tSTRING_TYPE;
"null"                        return tNULLPTR;

  /* CICLES */

"while"                       return tWHILE;
"do"                          return tDO;
"finally"                     return tFINALLY; 

  /* CONDITIONS */

"if"                          return tIF;
"then"                        return tTHEN; 
"else"                        return tELSE; 

  /* IDENTIFIERS */

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

  /* COMMENTS */

  /* MISSING SOME CASES - SEE WIKI EXERCICES*/
"--".*                          ; // ignore comments 

"/*"                            yy_push_state(X_COMMENT);
<X_COMMENT>"/*"                 yy_push_state(X_COMMENT);
<X_COMMENT>"*/"                 yy_pop_state();
<X_COMMENT>.|\n                 ; /* ignore content */

  /* STRINGS */

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\'           yy_pop_state(); return tSTRING;
<X_STRING>\\\'         *yylval.s += yytext + 1;
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

  /* INTEGERS */

[0-9]+                 yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

  /* FLOATS */

  /*(...)*/

  /* OTHERS */

[-()<>=+*/%;{}.]       return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
