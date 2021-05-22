%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"

// output stream for building string literals
static std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}

FLOAT        ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?
LETTER       [A-Za-z_]
IDENTIFIER   {LETTER}({LETTER}|[[:digit:]])*
SPACE        [ \t\r\n]

%x X_STRING X_COMMENT X_SPECIALCHAR X_OCTAL
%%

                                    {yydebug=0;}

  /* OPERATORS */

"sizeof"                            return tSIZEOF;

">="                                return tGE;
"<="                                return tLE;
"=="                                return tEQ;
"!="                                return tNE;

"&&"                                return tAND;
"||"                                return tOR;

  /* SKIP OPERATORS */

"leave"                             return tLEAVE;
"restart"                           return tRESTART;
"return"                            return tRETURN;

  /*PROLOGUE*/

@                                   return *yytext;


  /*EPILOGUE*/
>                                   return *yytext;

  /*DEFAULT RETURN*/

"->"                                return tDEFAULT;


  /* INPUT OUTPUT */

"write"                             return tWRITE;
"writeln"                           return tWRITELN;

  /* KEYWORDS */

"void"                              return tVOID_TYPE;
"int"                               return tINT_TYPE;
"float"                             return tFLOAT_TYPE;
"string"                            return tSTRING_TYPE;
"null"                              return tNULLPTR;

  /* CICLES */

"while"                             return tWHILE;
"do"                                return tDO;
"finally"                           return tFINALLY;

  /* CONDITIONS */

"if"                                return tIF;
"then"                              return tTHEN;
"else"                              return tELSE;

  /* IDENTIFIERS */

{IDENTIFIER}                        yylval.s = new std::string(yytext); return tIDENTIFIER;

  /* COMMENTS */

"!!".*\n                            ; // ignore comments

"(*"                                yy_push_state(X_COMMENT);
<X_COMMENT>"(*"                     yy_push_state(X_COMMENT);
<X_COMMENT>"*)"                     yy_pop_state();
<X_COMMENT>.|\n                     ; /* ignore content */

  /* STRINGS */

\'                                  yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\~                        yy_push_state(X_SPECIALCHAR);
<X_STRING>\'                        {
                                    yylval.s = new std::string(strlit.str());
                                    strlit.str("");
                                    yy_pop_state();
                                    return tSTRING;
                                    }
<X_STRING>.                         strlit << *yytext;
<X_STRING>\n                        yyerror("newline in string");

<X_SPECIALCHAR>n                    { strlit << '\n'; yy_pop_state(); }
<X_SPECIALCHAR>r                    { strlit << '\r'; yy_pop_state(); }
<X_SPECIALCHAR>t                    { strlit << '\t'; yy_pop_state(); }
<X_SPECIALCHAR>\'                   { strlit << "~'"; yy_pop_state(); }
<X_SPECIALCHAR>\~                   { strlit << "~~"; yy_pop_state(); }
<X_SPECIALCHAR>[[:xdigit:]]{1,2}    {
                                    strlit << (char)(unsigned char)std::stoul(yytext, NULL, 16);
                                    yy_pop_state();
                                    }
<X_SPECIALCHAR>0[[:alnum:]]*        {
                                    strlit << '\0';
                                    yy_pop_state();
                                    }
<X_SPECIALCHAR>.                    { strlit << *yytext; yy_pop_state(); }

  /* INTEGERS */
  /*Octal*/
"0"                           yy_push_state(X_OCTAL); 
<X_OCTAL>[0-7]+                    { yylval.i = strtoul(yytext, NULL, 8); yy_pop_state(); return tINTEGER; }
<X_OCTAL>[0-9]+                     std::cerr << "WARNING: line " << lineno() << ": bad Octogonal representation!" << std::endl; exit(1);

[1-9][0-9]*                              yylval.i = strtol(yytext, NULL, 10); return tINTEGER;

  /* FLOATS */



"."[0-9]+ {try {yylval.d = std::stod(yytext, NULL);
                double d = std::stod(yytext, NULL);
                if(int(d) == d){
                  yylval.i = int(d);
                  return tINTEGER;
                }
                return tREAL;
                } catch (std::out_of_range& e) {yyerror("Real out of range.");}
          }

[0-9]+"." { try { yylval.d = std::stod(yytext, NULL);
                double d = std::stod(yytext, NULL);
                if(int(d) == d){
                  yylval.i = int(d);
                  return tINTEGER;
                }
                return tREAL;
                } catch (std::out_of_range& e) {yyerror("Real out of range."); }
            }

[0-9]+"."[0-9]+ {try{yylval.d = std::stod(yytext, NULL);
                  double d = std::stod(yytext, NULL);
                  if(int(d) == d){
                    yylval.i = int(d);
                    return tINTEGER;
                  }
                  return tREAL;
                  }catch (std::out_of_range& e) { yyerror("Real out range."); }
                }

[0-9]+("."[0-9]+)?[Ee][-+]?[0-9]+  {try { yylval.d = std::stod(yytext, NULL);
                                          double d = std::stod(yytext, NULL);
                                          if(int(d) == d){
                                            yylval.i = int(d);
                                            return tINTEGER;
                                          }
                                          return tREAL;
                                          } catch(std::out_of_range& e) {yyerror("Real out of range.");}
                                    }





  /* OTHERS */
[\[\]{};:!(),\-+*/%~<>@?=]           return *yytext;

{SPACE}+                            ; /* ignore whitespace */

.                                   yyerror("Unknown character");

%%
