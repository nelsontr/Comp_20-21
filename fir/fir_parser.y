%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                  i;	/* integer value */
  double               d;    /* float values */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

	fir::block_node      *block;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING
%token tIF tTHEN tWHILE tDO 
%token tWRITELN tWRITE tINPUT 
%token tOR  tAND tRETURN tRESTART tLEAVE tSIZEOF tDEFAULT
%token tVOID_TYPE tINT_TYPE tFLOAT_TYPE tSTRING_TYPE tNULLPTR
%token tGIVEN tAPPLY tTO tTIMES


%nonassoc tIF
%nonassoc tTHEN
%nonassoc tELSE
%nonassoc tFINALLYX
%nonassoc tFINALLY

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%nonassoc '(' ')' '[' ']'

%type <node> file decl variableDec funcDec funcDef arg instruction given
%type <sequence> decls variableDecs optVariableDec args instructions optionalInstruc exprs opt_exprs body body1 body2
%type <expression> expr optDefault
%type <s> string
%type <lvalue> lval
%type <type> data_type void_type
%type <block> block epil prolg


%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file       : /* empty */                                       { compiler->ast($$ = new cdk::sequence_node(LINE)); }
			     | decls                                             { compiler->ast($$ = $1); }
			     ;

decls              : decl                                      { $$ = new cdk::sequence_node(LINE, $1); }
                   | decls decl                                { $$ = new cdk::sequence_node(LINE, $2, $1); }
                   ;

decl               : variableDec ';'    	                     { $$ = $1; }
                   | funcDec     			                         { $$ = $1; }
                   | funcDef     			                         { $$ = $1; }
                   ;

variableDec       : data_type '*' tIDENTIFIER                  { $$ = new fir::declaration_variable_node(LINE, 1,  $1, *$3, nullptr); }
                  | data_type '*' tIDENTIFIER '=' expr         { $$ = new fir::declaration_variable_node(LINE, 1,  $1, *$3, $5); }
                  | data_type '?' tIDENTIFIER                  { $$ = new fir::declaration_variable_node(LINE, 2, $1, *$3, nullptr); }
                  | data_type  tIDENTIFIER                     { $$ = new fir::declaration_variable_node(LINE, 0, $1, *$2, nullptr); }
                  | data_type  tIDENTIFIER '=' expr            { $$ = new fir::declaration_variable_node(LINE, 0, $1, *$2, $4); }
                  ;

variableDecs       : variableDec ';'                           { $$ = new cdk::sequence_node(LINE, $1);     }
                   | variableDec ';' variableDecs              { $$ = new cdk::sequence_node(LINE, $1, $3); }
                   ;

optVariableDec     : /* empty */                               { $$ = nullptr; }
                   | variableDecs                              { $$ = $1; }
                   ;

data_type          : tSTRING_TYPE                              { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
                   | tINT_TYPE                                 { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
                   | tFLOAT_TYPE                               { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
                   | '<' data_type '>'                         { $$ = cdk::reference_type::create(4, $2); }
                   ;

void_type          : tVOID_TYPE                                { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID); }

                   ;
funcDec            :     data_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 0,  $1, *$2, $4); delete $2; }
                   | data_type '*'  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 1,  $1, *$3, $5); delete $3; }
                   | data_type '?'  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 2,  $1, *$3, $5); delete $3; }
                   |     void_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 0, *$2, $4); delete $2; }
                   | void_type '*'  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 1, *$3, $5); delete $3; }
                   | void_type '?'  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 2, *$3, $5); delete $3; }
                   ;

funcDef            :     data_type  tIDENTIFIER '(' args ')' optDefault body       { $$ = new fir::function_definition_node(LINE, 0,  $1, *$2, $4, $6, $7); delete $2; }
                   | data_type '*'  tIDENTIFIER '(' args ')' optDefault body       { $$ = new fir::function_definition_node(LINE, 1,  $1, *$3, $5, $7, $8); delete $3; }
                   |     void_type  tIDENTIFIER '(' args ')' optDefault body       { $$ = new fir::function_definition_node(LINE, 0, *$2, $4, $6, $7); delete $2; }
                   | void_type '*'  tIDENTIFIER '(' args ')' optDefault body       { $$ = new fir::function_definition_node(LINE, 1, *$3, $5, $7, $8); delete $3; }
                   ;
                   
args               : /* empty */         	                     { $$ = new cdk::sequence_node(LINE);  }
                   | arg                                       { $$ = new cdk::sequence_node(LINE, $1);     }
                   | args ',' arg                              { $$ = new cdk::sequence_node(LINE, $3, $1); }
                   ;

arg                : data_type tIDENTIFIER                     { $$ = new fir::declaration_variable_node(LINE, 0, $1, *$2, nullptr); }
                   ;

optDefault         :    /* empty */                            { $$ = nullptr; }
                   | tDEFAULT tINTEGER                         { $$ = new cdk::integer_node(LINE, $2); }
                   | tDEFAULT tREAL                            { $$ = new cdk::double_node(LINE, $2); }
                   | tDEFAULT string                           { $$ = new cdk::string_node(LINE, $2); }
                   ;

body               : prolg body1                               { $$ = new cdk::sequence_node(LINE, $1); }
                   | block body2                               { $$ = new cdk::sequence_node(LINE, $1); }
                   | epil                                      { $$ = new cdk::sequence_node(LINE, $1); }
                   ;

body1              : /*empty*/                                 { $$ = nullptr;}
                   | block body2                               { $$ = new cdk::sequence_node(LINE, $1); }
                   ;

body2              : /*empty*/                                 { $$ = new cdk::sequence_node(LINE);}
                   | epil                                      { $$ = new cdk::sequence_node(LINE, $1); }
                   ;

prolg              : '@' block                                 { $$ = $2; }
                   ;

epil               : '>' '>' block                             { $$ = $3; }
                   ;

block             : '{' optVariableDec optionalInstruc '}'                    { $$ = new fir::block_node(LINE, $2, $3); }             
                  ;

instruction        : tIF expr tTHEN instruction                               { $$ = new fir::if_node(LINE, $2, $4); }
                   | tIF expr tTHEN instruction tELSE instruction             { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
                   | tWHILE expr tDO instruction %prec tFINALLYX              { $$ = new fir::while_node(LINE, $2, $4); }
                   | tWHILE expr tDO instruction tFINALLY instruction         { $$ = new fir::while_finally_node(LINE, $2, $4, $6); }
                   | given                                                     {$$ = $1; }  
                   | expr ';'                                                 { $$ = new fir::evaluation_node(LINE, $1); }
                   | tWRITE   exprs ';'                                       { $$ = new fir::write_node(LINE, $2, false); }
                   | tWRITELN exprs ';'                                       { $$ = new fir::write_node(LINE, $2, true); }
                   | tLEAVE ';'                                               { $$ = new fir::leave_node(LINE);  }
                   | tLEAVE tINTEGER ';'                                      { $$ = new fir::leave_node(LINE, $2);  }
                   | tRESTART tINTEGER ';'                                    { $$ = new fir::restart_node(LINE, $2); }
                   | tRESTART ';'                                             { $$ = new fir::restart_node(LINE); }
                   | tRETURN                                                  { $$ = new fir::return_node(LINE); }
                   | block                                                    { $$ = $1; }
                   ;

given              : tGIVEN expr tAPPLY tIDENTIFIER tTO tIDENTIFIER tTIMES tINTEGER {$$ = new fir::given_apply_node(LINE, $2, *$4, *$6, $8); }  
                   ;

instructions       : instruction                      { $$ = new cdk::sequence_node(LINE, $1);     }
                   | instructions instruction         { $$ = new cdk::sequence_node(LINE, $2, $1); }
                   ;

optionalInstruc    : /* empty */                      { $$ = new cdk::sequence_node(LINE); }
                   | instructions                     { $$ = $1; }
                   ;



  /*nao se tem que fazer uma */
expr               : tINTEGER                         { $$ = new cdk::integer_node(LINE, $1);}
                   | tREAL                            { $$ = new cdk::double_node(LINE, $1); }
                   | string                           { $$ = new cdk::string_node(LINE, $1); }
                   | tNULLPTR                         { $$ = new fir::null_pointer_node(LINE); }
                   /* LEFT VALUES */
                   | lval                             { $$ = new cdk::rvalue_node(LINE, $1); }
                   /* ARITHMETIC EXPRESSIONS */
                   | expr '+' expr	                  { $$ = new cdk::add_node(LINE, $1, $3); }
                   | expr '-' expr	                  { $$ = new cdk::sub_node(LINE, $1, $3); }
                   | expr '*' expr	                  { $$ = new cdk::mul_node(LINE, $1, $3); }
                   | expr '/' expr	                  { $$ = new cdk::div_node(LINE, $1, $3); }
                   | expr '%' expr	                  { $$ = new cdk::mod_node(LINE, $1, $3); }
                   /* UNARY EXPRESSION */
                   | '-' expr %prec tUNARY            { $$ = new cdk::neg_node(LINE, $2); }
                   | '+' expr %prec tUNARY            { $$ = $2; }
                   | '~' expr                         { $$ = new cdk::not_node(LINE, $2); }
                   /* OTHER EXPRESSION */
                   | '@'                              { $$ = new fir::read_node(LINE); }
                   | tSIZEOF '(' expr ')'             { $$ = new fir::size_of_node(LINE, $3); }
                   | tIDENTIFIER '(' opt_exprs ')'    { $$ = new fir::function_call_node(LINE, *$1 , $3); delete $1; }
                   /* LOGICAL EXPRESSIONS */
                   | expr '<' expr	                  { $$ = new cdk::lt_node(LINE, $1, $3); }
                   | expr '>' expr	                  { $$ = new cdk::gt_node(LINE, $1, $3); }
                   | expr tGE expr	                  { $$ = new cdk::ge_node(LINE, $1, $3); }
                   | expr tLE expr                    { $$ = new cdk::le_node(LINE, $1, $3); }
                   | expr tNE expr	                  { $$ = new cdk::ne_node(LINE, $1, $3); }
                   | expr tEQ expr	                  { $$ = new cdk::eq_node(LINE, $1, $3); }
                   | expr tAND expr	                  { $$ = new cdk::and_node(LINE, $1, $3);}
                   | expr tOR expr	                  { $$ = new cdk::or_node(LINE, $1, $3); }
                   /* ASSIGNMENTS */
                   | lval '=' expr                    { $$ = new cdk::assignment_node(LINE, $1, $3); }
                   /* OTHER EXPRESSION */
                   | '(' expr ')'                     { $$ = $2; }
                   | '[' expr ']'                     { $$ = new fir::alloc_node(LINE, $2); }
                   | lval '?'                         { $$ = new fir::address_of_node(LINE, $1);}
                   ;
                   

exprs              : expr                             { $$ = new cdk::sequence_node(LINE, $1);     }
                   | exprs ',' expr                   { $$ = new cdk::sequence_node(LINE, $3, $1); }
                   ;

opt_exprs          : /* empty */                      { $$ = new cdk::sequence_node(LINE); }
                   | exprs                            { $$ = $1; }
                   ;


string             : tSTRING                          { $$ = $1; }
                   | string tSTRING                   { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
                   ;
                   
lval               : tIDENTIFIER                                  { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
                   | lval '[' expr ']'                            { $$ = new fir::pointer_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
                   | '(' expr ')' '[' expr ']'                    { $$ = new fir::pointer_node(LINE, $2, $5); }
                   | tIDENTIFIER '(' opt_exprs ')' '[' expr ']'   { $$ = new fir::pointer_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); }
                   ;

%%
