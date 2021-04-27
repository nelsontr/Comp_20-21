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

  int                   i;	/* integer value */
  float                 f;    /* float values */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

	fir::block_node       *block;
};

%token <i> tINTEGER
%token <f> tFLOAT
%token <s> tIDENTIFIER tSTRING
%token tIF tTHEN tWHILE tDO tFINALLY
%token tWRITELN tWRITE tINPUT tOR  tAND
%token tRETURN tRESTART tLEAVE tSIZEOF
%token tVOID_TYPE tINT_TYPE tFLOAT_TYPE tSTRING_TYPE tNULLPTR

%nonassoc tIF
%nonassoc tTHEN
%nonassoc tELSE

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

%type <node> stmt //program
%type <sequence> list exprsFor
%type <expression> expr
%type <lvalue> lval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file  : /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); } 
			| declarations { compiler->ast($$ = $1); }
			;

declarations : declaration              { $$ = new cdk::sequence_node(LINE, $1); }
             | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;


declaration : variableDec ';'    	{$$ = $1; }
            | funcDec     				{$$ = $1; }
            | funcDef     				{$$ = $1; }
            ;

variableDec    : '*' data_type  tIDENTIFIER                    { $$ = new fir::declaration_variable_node(LINE, 1,  $2, *$3, nullptr); }
               | '*' data_type  tIDENTIFIER opt_initializer    { $$ = new fir::declaration_variable_node(LINE, 1,  $2, *$3, $4); }
               | '?' data_type  tIDENTIFIER                    { $$ = new fir::declaration_variable_node(LINE, 2, $2, *$3, nullptr); }
               | '?' data_type  tIDENTIFIER opt_initializer    { $$ = new fir::declaration_variable_node(LINE, 2, $2, *$3, $4); }
               | data_type  tIDENTIFIER                        { $$ = new fir::declaration_variable_node(LINE, 0, $1, *$2, nullptr); }
               | data_type  tIDENTIFIER opt_initializer        { $$ = new fir::declaration_variable_node(LINE, 0, $1, *$2, $3); }
               ;

variableDecs : variableDec ';'          { $$ = new cdk::sequence_node(LINE, $1);     }
             | variableDecs variableDec ';' { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

optionalVariableDec      : /* empty */ { $$ = NULL; }
                         | variableDecs     { $$ = $1; }
                         ;

opt_initializer  : /* empty */         	{ $$ = nullptr; }
                 | '=' expr      				{ $$ = $2; }
                 ;

data_type    : tSTRING_TYPE                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
             | tINT_TYPE                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
             | tFLOAT_TYPE                      { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
             | '<' data_type '>'                { $$ = cdk::reference_type::create(4, $2); }
             ;

void_type   : tVOID_TYPE { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);   }
            ;

fundec   : data_type  tIDENTIFIER '(' args ')'                      { $$ = new fir::function_declaration_node(LINE, 0, $1, *$2, $4);  }
         | data_type  tIDENTIFIER '(' args ')' tDEFAULT literal     { $$ = new fir::function_declaration_node(LINE, 1,  $2, *$3, $5); }
         | '*' data_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 1,  $2, *$3, $5); }
         | '*' data_type  tIDENTIFIER '(' args ')' tDEFAULT literal { $$ = new fir::function_declaration_node(LINE, 1,  $2, *$3, $5); }
         | '?' data_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 2,  $2, *$3, $5); }
         | void_type  tIDENTIFIER '(' args ')'											{ $$ = new fir::function_declaration_node(LINE, 0, $1, *$2, $4);  }
         | '*' void_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 1,  $2, *$3, $5); }
         | '?' void_type  tIDENTIFIER '(' args ')'                  { $$ = new fir::function_declaration_node(LINE, 2,  $2, *$3, $5); }
         ;

args  : /* empty */         	{ $$ = new cdk::sequence_node(LINE);  }
      | arg                  	{ $$ = new cdk::sequence_node(LINE, $1);     }
      | args ',' arg         	{ $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;

arg  : data_type tID { $$ = new fir::declaration_variable_node(LINE, tPRIVATE, $1, *$2, nullptr); }
     ;

block     : '{' optionalVariableDec optionalInstruc '}' { $$ = new fir::block_node(LINE, $2, $3); }
          ;

return    : tRETURN ';'            { $$ = new fir::return_node(LINE, nullptr); }
          | tRETURN exprs ';'      { $$ = new fir::return_node(LINE, $2); }
          | tDEFAULT literal       { $$ = new fir::return_node(LINE, $2); }          //TER CUIDADO COM ISTO
          ;

instruction     : tIF expr tTHEN instruction                                          { $$ = new fir::if_node(LINE, $2, $4); }
                | tIF expr tTHEN instruction tELSE instruction                        { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
                | tWHILE expr tDO instruction                                         { $$ = new fir::while_node(LINE, $2, $4); }
                | tWHILE expr tDO instruction tFINALLY instructions                   { $$ = new fir::while_finnaly_node(LINE, $2, $4, $6); }
                | expr ';'                                                            { $$ = new fir::evaluation_node(LINE, $1); }
                | tWRITE   exprs ';'                                                  { $$ = new fir::write_node(LINE, $2, false); }
                | tWRITELN exprs ';'                                                  { $$ = new fir::write_node(LINE, $2, true); }
                | tLEAVE                                                              { $$ = new fir::leave_node(LINE);  }
                | tRESTART                                                            { $$ = new fir::restart_node(LINE); }
                | return                                                              { $$ = $1; }
                | block                                                               { $$ = $1; }
                ;

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

optionalInstruc: /* empty */  { $$ = new cdk::sequence_node(LINE); }
                | instructions { $$ = $1; }
                ;


lvalue    : tIDENTIFIER                                { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
          | lvalue '[' expr ']'                        { $$ = new fir::pointer_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
          | '(' expr ')' '[' expr ']'                  { $$ = new fir::pointer_node(LINE, $2, $5); }
          | tIDENTIFIER '(' opt_exprs ')' '[' expr ']' { $$ = new fir::pointer_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); }
          ;

expr : integer                    { $$ = $1; }
     | real                    { $$ = $1; }
     | string                          { $$ = new cdk::string_node(LINE, $1); }
     | tNULLPTR                   { $$ = new fir::null_pointer_node(LINE);}     
     /* LEFT VALUES */
     | lval                       { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     /* ASSIGNMENTS */
     | lval '=' expr              { $$ = new cdk::assignment_node(LINE, $1, $3); }
     /* ARITHMETIC EXPRESSIONS */
     | expr '+' expr	                   { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	                   { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	                   { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	                   { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	                   { $$ = new cdk::mod_node(LINE, $1, $3); }
     /* LOGICAL EXPRESSIONS */
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr              { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr tAND expr	         { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr tOR expr	         { $$ = new cdk::or_node(LINE, $1, $3); }
     /* UNARY EXPRESSION */
     | '-' expr %prec tUNARY                	{ $$ = new cdk::neg_node(LINE, $2); }
     | '+' expression %prec tUMINUS          	{ $$ = $2; }
     | '~' expr                              	{ $$ = new cdk::not_node(LINE, $2); }
     /* OTHER EXPRESSION */
     | tINPUT                       { $$ = new fir::read_node(LINE); }
     | tSIZEOF '(' exprs ')'            { $$ = new fir::sizeof_node(LINE, new $3}
     | tIDENTIFIER '(' exprsFor ')'      { $$ = new fir::function_call_node(LINE, *$1 , $3); delete $1; }
     | '*' tIDENTIFIER '(' exprsFor ')'      { $$ = new fir::function_call_node(LINE, *$2 , $4); delete $2; }
     | '?' tIDENTIFIER '(' exprsFor ')'      { $$ = new fir::function_call_node(LINE, *$2 , $4); delete $2; }     
     /* OTHER EXPRESSION */
     | '(' expr ')'               { $$ = $2; }
     | '[' expr ']'               { $$ = new fir::???(LINE, $2); }
     | lval '?'              { $$ = new fir::address_of_node(LINE, $1);}
     ;

identifiers  : tIDENTIFIER { $$ = new std::vector<std::string>(); $$->push_back(*$1); delete $1; }
             | identifiers ',' tIDENTIFIER { $$ = $1; $$->push_back(*$3); delete $3; }

exprs     : expr                          { $$ = new cdk::sequence_node(LINE, $1);     }
          | exprs ',' expr                { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

opt_exprs : /* empty */                   { $$ = new cdk::sequence_node(LINE); }
          | exprs                         { $$ = $1; }
          ;

literal   : integer      { $$ = $1; }
          | real         { $$ = $1; }
          | string       { $$ = $1; }
          ;

integer   : tINTEGER                      { $$ = new cdk::integer_node(LINE, $1); };
real      : tREAL                         { $$ = new cdk::double_node(LINE, $1); };
string    : tSTRING                       { $$ = $1; }
          | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
          ;

%%
