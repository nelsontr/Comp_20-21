#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h" // all.h is automatically generated

//---------------------------------------------------------------------------

bool _insideFunction = false;

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
  if (_insideFunction)
    _pf.DOUBLE(node->value()); // push an integer
  else
    _pf.SDOUBLE(node->value()); // store an integer
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
  int lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl = ++_lbl));

  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void fir::postfix_writer::do_or_node(cdk::or_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl = ++_lbl));

  node->right()->accept(this, lvl + 2);
  _pf.AND();
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
  if (_insideFunction)
    _pf.INT(node->value()); // push an integer
  else
    _pf.SINT(node->value()); // store an integer
}

void fir::postfix_writer::do_string_node(cdk::string_node *const node, int lvl)
{
  int lbl1;

  /* generate the string */
  _pf.RODATA();                    // strings are DATA readonly
  _pf.ALIGN();                     // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value());      // output string characters

  /* leave the address on the stack */
  _pf.TEXT();            // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_neg_node(cdk::neg_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DNEG();
  else
    _pf.NEG(); // 2-complement
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
  _pf.EQ();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  // WHAT IS _symtab
  // simplified generation: all variables are global
  _pf.ADDR(node->name());
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);

  if (node->lvalue()->is_typed(cdk::TYPE_INT) ||
      node->lvalue()->is_typed(cdk::TYPE_STRING) ||
      node->lvalue()->is_typed(cdk::TYPE_POINTER))
    _pf.LDINT();
  else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE))
    _pf.LDDOUBLE();
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();
  if (new_symbol() == nullptr)
    node->lvalue()->accept(this, lvl); // where to store the value
  else
  {
    _pf.DATA();                      // variables are all global and live in DATA
    _pf.ALIGN();                     // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    reset_new_symbol();
    _pf.SINT(0);                       // initialize it to 0 (zero)
    _pf.TEXT();                        // return to the TEXT segment
    node->lvalue()->accept(this, lvl); //DAVID: bah!
  }
  _pf.STINT(); // store the value at address
}

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT))
  {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
  {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else
  {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  _pf.STINT();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//CÓDIGO NOVO

void fir::postfix_writer::do_return_node(fir::return_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_leave_node(fir::leave_node *const node, int lvl)
{
  /*if (_whileEnd.size() != 0)
    _pf.JMP(mklbl(_whileEnd.top()));
  else
    throw new std::string("Cannot perform a break outside a 'for' loop.");
*/}
void fir::postfix_writer::do_while_finally_node(fir::while_finally_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_restart_node(fir::restart_node *const node, int lvl)
{
  /*if (_whileStart.size() != 0)
    _pf.JMP(mklbl(_whileStart.top()));
  else
    throw new std::string("Cannot perform a continue outside a 'for' loop.");
*/}
void fir::postfix_writer::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_function_call_node(fir::function_call_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_function_declaration_node(fir::function_declaration_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_function_definition_node(fir::function_definition_node *const node, int lvl)
{
  // EMPTY
}
void fir::postfix_writer::do_identify_node(fir::identify_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void fir::postfix_writer::do_null_pointer_node(fir::null_pointer_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;

  if (_insideFunction)
    _pf.INT(0);
  else
    _pf.SINT(0);
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
  //_symtab.push();
  if (node->declarations())
    node->declarations()->accept(this, lvl + 2);
  if (node->instructions())
    node->instructions()->accept(this, lvl + 2);
  //_symtab.pop();
}
void fir::postfix_writer::do_write_node(fir::write_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);

  //VER FOR ANTERIOR

  /*if (node->argument()->is_typed(cdk::TYPE_INT))
  {
    //_symbols_to_declare.insert("printi");
    _pf.CALL("printi");
    _pf.TRASH(4);
  }
  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE))
  {
    //_symbols_to_declare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
  {
    //_symbols_to_declare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4);
  }*/
}
void fir::postfix_writer::do_alloc_node(fir::alloc_node *const node, int lvl)
{
  ASSERT_SAFE_EXPRESSIONS;
  //size_t typeSize = cdk::reference_type_cast(node->type())->referenced()->size();

  node->argument()->accept(this, lvl + 2);
 // _pf.INT(typeSize);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}
