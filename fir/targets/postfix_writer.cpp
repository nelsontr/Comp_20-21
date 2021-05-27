#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h" // all.h is automatically generated

//---------------------------------------------------------------------------

void fir::postfix_writer::do_nil_node(cdk::nil_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_data_node(cdk::data_node *const node, int lvl)
{
  // EMPTY
}

void fir::postfix_writer::do_double_node(cdk::double_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  if (_insideFunction) _pf.DOUBLE(node->value());
  else _pf.SDOUBLE(node->value());
}

void fir::postfix_writer::do_not_node(cdk::not_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.NOT();
}

void fir::postfix_writer::do_and_node(cdk::and_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));

  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void fir::postfix_writer::do_or_node(cdk::or_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));

  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sequence_node(cdk::sequence_node *const node, int lvl)
{
  for (size_t i = 0; i < node->size(); i++)
  {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_integer_node(cdk::integer_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  if (_insideFunction) _pf.INT(node->value());
  else _pf.SINT(node->value());
}

void fir::postfix_writer::do_string_node(cdk::string_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl;

  /* generate the string */
  _pf.RODATA();                    // strings are DATA readonly
  _pf.ALIGN();                     // make sure we are aligned
  _pf.LABEL(mklbl(lbl = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value());      // output string characters

  if (_insideFunction) {
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl));
  }
  else {
    _pf.DATA(); // return to the DATA segment
    _pf.SADDR(mklbl(lbl)); // the string to be printed
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_neg_node(cdk::neg_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);

  if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DNEG();
  else _pf.NEG();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_add_node(cdk::add_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void fir::postfix_writer::do_sub_node(cdk::sub_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void fir::postfix_writer::do_mul_node(cdk::mul_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void fir::postfix_writer::do_div_node(cdk::div_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void fir::postfix_writer::do_mod_node(cdk::mod_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void fir::postfix_writer::do_lt_node(cdk::lt_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void fir::postfix_writer::do_le_node(cdk::le_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void fir::postfix_writer::do_ge_node(cdk::ge_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void fir::postfix_writer::do_gt_node(cdk::gt_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void fir::postfix_writer::do_ne_node(cdk::ne_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void fir::postfix_writer::do_eq_node(cdk::eq_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);

  if ((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
    || (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))) {
    _pf.I2D();
  }
  else if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<fir::symbol> symbol = _symtab.find(node->name());
  
  if (!symbol->offset()) _pf.ADDR(symbol->name());
  else _pf.LOCAL(symbol->offset());
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);

  if (INT_STRING_POINTER) _pf.LDINT();
  else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) _pf.LDDOUBLE();
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  
  node->rvalue()->accept(this, lvl);

  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    if ( node->rvalue()->type()->name() == cdk::TYPE_INT ) _pf.I2D();
    _pf.DUP64();
    node->lvalue()->accept(this, lvl);
    _pf.STDOUBLE();
  } else {
    _pf.DUP32();
    node->lvalue()->accept(this, lvl);
    _pf.STINT();
  }
}

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  if (node->argument()->type()->name() != cdk::TYPE_VOID)
      _pf.TRASH(node->argument()->type()->size());
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  if (node->type()->name() == cdk::TYPE_INT) {
    _pf.CALL("readi");
    _pf.LDFVAL32();
  }
  else if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.CALL("readd");
    _pf.LDFVAL64();
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int while_cond = ++_lbl;
  int while_end = ++_lbl;
  _whileCondition.push(while_cond);
  _whileEnd.push(while_end);

  _pf.LABEL(mklbl(while_cond));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(while_end));

  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(while_cond));
  _pf.LABEL(mklbl(while_end));
  _whileEnd.pop();
  _whileCondition.pop();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1 = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1));

  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1 = ++_lbl, lbl2 = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1));

  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2));
  _pf.LABEL(mklbl(lbl1));

  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void fir::postfix_writer::do_return_node(fir::return_node *const node, int lvl)
{
  // EMPTY
}

void fir::postfix_writer::do_leave_node(fir::leave_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  for(int lvl = node->lvl(); lvl>1; lvl--){
    _whileEnd.pop();
  }
  if (_whileEnd.size()) _pf.JMP(mklbl(_whileEnd.top()));
  else throw new std::string("Cannot perform a break outside a 'for' loop.");
}

void fir::postfix_writer::do_while_finally_node(fir::while_finally_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int while_cond = ++_lbl, end_label = ++_lbl;

  _pf.LABEL(mklbl(while_cond));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(end_label));

  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(while_cond));
  _pf.LABEL(mklbl(end_label));
}

void fir::postfix_writer::do_restart_node(fir::restart_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  if (_whileCondition.size()) _pf.JMP(mklbl(_whileCondition.top()));
  else throw new std::string("Cannot perform a continue outside a 'for' loop.");
}

void fir::postfix_writer::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  std::vector<std::string> *identifiers = new std::vector<std::string>();
  std::string id = node->variableId();
  std::string delimiter = ",";
  size_t pos = 0;
  std::string token;

  while ((pos =  id.find(delimiter)) != std::string::npos) {
      identifiers->push_back(id.substr(0, pos));
      id.erase(0, pos + delimiter.length());
  }

  identifiers->push_back(id);
  
  int offset = 0;
  int size = node->type()->size();

  if(_insideFunction) {
    _offset -= size;
    offset = _offset;
  }
  else if(_insideFunctionArgs) {
    offset = _offset;
    _offset += size;
  }
  std::shared_ptr<fir::symbol> symbol = new_symbol();

  if(symbol) {
    symbol->offset(offset);
    reset_new_symbol();
  }

  if (node->initializer()) {
    if(_insideFunction) { 
      node->initializer()->accept(this, lvl);
      if(node->is_typed(cdk::TYPE_DOUBLE)) {
        if(node->initializer()->is_typed(cdk::TYPE_INT)) _pf.I2D();
        _pf.LOCAL(offset);
        _pf.STDOUBLE();
      }
      else {
        _pf.LOCAL(offset);
        _pf.STINT();
      }
    }
    else if (!_insideFunction && !_insideFunctionArgs)
    {
      if(INT_DOUBLE_POINTER) {
        _pf.DATA();
        _pf.ALIGN();
        if (node->qualifier() == 1) {
          _pf.GLOBAL(id, _pf.OBJ());
        }
        _pf.LABEL(id);

        if(INT_POINTER) { node->initializer()->accept(this, lvl); }
        else if (node->is_typed(cdk::TYPE_DOUBLE)){
          if(node->initializer()->is_typed(cdk::TYPE_DOUBLE)) node->initializer()->accept(this, lvl);
          else if(node->initializer()->is_typed(cdk::TYPE_INT)) {
            cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->initializer());
            cdk::double_node ddi(dclini->lineno(), dclini->value());
            ddi.accept(this, lvl);
          }
          else {
            std::cerr << node->lineno() << ": '" << id << "' wrong initializer for real variable.\n";
            exit(2);
          }
        }
      }
      else if (node->is_typed(cdk::TYPE_STRING)) {
        _pf.DATA(); 
        _pf.ALIGN();
        _pf.LABEL(node->variableId());
        node->initializer()->accept(this, lvl); 
      }
    }
    else {
      std::cerr << node->lineno() << ": '" << id << "' has an unexpected initializer.\n";
      exit(2);
    }
  }
  else {
      if (!_insideFunction && !_insideFunctionArgs && INT_DOUBLE_POINTER_STRING)
      {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(size);
      }
  }
}

void fir::postfix_writer::do_function_call_node(fir::function_call_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  size_t args_size = 0;
  std::shared_ptr<fir::symbol> symbol = _symtab.find(node->identifier());

  if (node->arguments()) {
    for (size_t i = node->arguments()->size(); i > 0; i--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i-1));
      std::shared_ptr<fir::symbol> param = symbol->params()->at(i-1);

      arg->accept(this, lvl + 2);
      args_size += param->type()->size();

      if (param->is_typed(cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) _pf.I2D();
    }
  }

  _pf.CALL(node->identifier());
  if (args_size != 0) {
    _pf.TRASH(args_size);
  }

  if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) {
    _pf.LDFVAL32();
  } else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  }
}

void fir::postfix_writer::do_function_declaration_node(fir::function_declaration_node *const node, int lvl)
{
  if (_insideFunction)
    throw new std::string("Cannot define function in function body or args");

  ASSERT_SAFE_EXPRESSIONS;
  auto symbol = _symtab.find(node->identifier());
  if(!symbol || !symbol->function()) {
      std::cerr << "Function not found. Should not happen" << std::endl;
      exit(1);
  }
  if(!symbol->isDefined()) {
      public_symbol(symbol->name());
  }
}

void fir::postfix_writer::do_function_definition_node(fir::function_definition_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  _function = _symtab.find(node->identifier());
  if (!_function) throw std::string("Symbol not found");

  _symtab.push();

  _pf.TEXT();
  _pf.ALIGN();

  if (node->qualifier() == 1)
    _pf.GLOBAL(node->identifier(), _pf.FUNC());

  _pf.LABEL(node->identifier());
  _pf.ENTER(0);

  if (node->arguments()) node->arguments()->accept(this, lvl);
  
  if (node->body()) {
    _insideFunction = true;
    node->body()->accept(this, lvl);
    _insideFunction = false;
  }

  if (node->identifier()=="_main"){
    _pf.INT(0);
    _pf.STFVAL32();
  }
  _pf.LEAVE();
  _pf.RET();

  _function = nullptr;
  _symtab.pop();

  // these are just a few library function imports
  if (node->identifier()=="_main"){
    _pf.EXTERN("readi");
    _pf.EXTERN("printi");
    _pf.EXTERN("printd");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
  }
}

void fir::postfix_writer::do_identify_node(fir::identify_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void fir::postfix_writer::do_null_pointer_node(fir::null_pointer_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  if (_insideFunction) _pf.INT(0);
  else _pf.SINT(0);
}

void fir::postfix_writer::do_pointer_node(fir::pointer_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

void fir::postfix_writer::do_size_of_node(fir::size_of_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->statement()->type()->size());
}

void fir::postfix_writer::do_address_of_node(fir::address_of_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl + 2);
}

void fir::postfix_writer::do_block_node(fir::block_node *const node, int lvl)
{
  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);  
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void fir::postfix_writer::do_write_node(fir::write_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  for(size_t i=0; i < node->argument()->size(); i++) {
    auto expression = (cdk::expression_node*) node->argument()->node(i);
    expression->accept(this, lvl);
    if (expression->is_typed(cdk::TYPE_INT)) {
      _pf.CALL("printi");
      _pf.TRASH(4);
    }
    else if (expression->is_typed(cdk::TYPE_STRING)) {
      _pf.CALL("prints");
      _pf.TRASH(4);
    }
    else if (expression->is_typed(cdk::TYPE_DOUBLE)) {
      _pf.CALL("printd");
      _pf.TRASH(8);
    }
    else throw new std::string("Cannot perform a break outside a 'for' loop.");
  }
  
  if(node->nLine())
    _pf.CALL("println");
}

void fir::postfix_writer::do_alloc_node(fir::alloc_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<cdk::reference_type> type = cdk::reference_type::cast(node->type());
  
  node->argument()->accept(this, lvl + 2);
  _pf.INT(type->referenced()->size());
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}
