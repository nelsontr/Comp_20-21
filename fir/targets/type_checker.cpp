#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for(size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_logical_operation(cdk::binary_operation_node *const node, int lvl){
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl +2);

  if(!node->left()->is_typed(cdk::TYPE_INT) || !node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("argument must type integer.");

  if(node->left()->is_typed(cdk::TYPE_UNSPEC)){
    fir::read_node *inputLeft = dynamic_cast<fir::read_node *>(node->left());
    if(inputLeft != nullptr)
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    else
      throw std::string("Specified node not found.");
  }

  if(node->right()->is_typed(cdk::TYPE_UNSPEC)){
    fir::read_node *inputRight = dynamic_cast<fir::read_node *>(node->right());
    if(inputRight != nullptr)
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    else
      throw std::string("Specified node not found.");
  }

}

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_INT))
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  else if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    fir::read_node *input = dynamic_cast<fir::read_node*>(node->argument());

    if(input != nullptr) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
    else
      throw std::string("Specified node not found.");
  }
  else
    throw std::string("Expected Integer type.");

}

void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_logical_operation(node, lvl);
}

void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_logical_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<fir::symbol>(cdk::primitive_type::create(4, cdk::TYPE_INT), id, 0);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT))
    throw std::string("Expected integer condition.");
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT))
    throw std::string("Expected integer condition.");
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT))
    throw std::string("Expected integer condition.");
}


void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
  // EMPTY feito
}
void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
  // EMPTY feito
}
void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
  // EMPTY feito
}
void fir::type_checker::do_identify_node(fir::identify_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_pointer_node(fir::pointer_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_write_node(fir::write_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);

  for (size_t i = 0; i < node->argument()->size(); i++) {
    auto type = dynamic_cast<cdk::expression_node*>(node->argument()->node(i))->type();
    if (type->name() == cdk::TYPE_POINTER)
      throw std::string("cannot print a pointer!");
  }
}


void fir::type_checker::do_while_finally_node(fir::while_finally_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT))
    throw std::string("Expected integer condition.");

}

void fir::type_checker::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl) {
  if(node->initializer())
    node->initializer()->accept(this, lvl+2);

  //tem problemas identifiers com virgulas

}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  if (node->identifier() == "fir")
    node->identifier("_main");
  else if (node->identifier() == "_main")
    node->identifier("._main");

  auto function = _symtab.find(node->identifier());

  if (!function) throw std::string("undeclared function '" + node->identifier() + "'");
  if (!function->function()) throw std::string(node->identifier() + "is not a function");
  if (!function->type()) throw std::string("could not infer function return type");

  node->type(function->type());
  if (node->arguments() && node->arguments()->size() != 0)
    node->arguments()->accept(this, lvl + 4);
  else if (node->arguments()->size() != function->params()->size())
    throw std::string("Function has different args size.");
  //pode ter mais
}

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
  if (node->identifier() == "fir")
    node->identifier("_main");
  else if (node->identifier() == "_main")
    node->identifier("._main");

  _function = symbol::make_symbol(node->type(), node->identifier(), true, node->qualifier(), node->arguments());

  auto existent_symbol = _symtab.find(node->identifier());

  if (existent_symbol == nullptr) {
    _symtab.insert(node->identifier(), _function);
    _parent->set_new_symbol(_function);
    return;
  }

  //IF already exists
  if (node->arguments()->size() == existent_symbol->params()->size()) {
    for (size_t i=0; i < node->arguments()->size(); i++) {
      cdk::typed_node* newArgument = (cdk::typed_node*) node->arguments()->node(i);
      if (newArgument->type()->name() != existent_symbol->params()->at(i)->type()->name())
        throw std::string("redecleared with different parameters");
    }
  }
  else throw std::string("redecleared with different number of parameters");

  //pode ter mais
}

void fir::type_checker::do_function_definition_node(fir::function_definition_node *const node, int lvl) {
  if (node->identifier() == "fir")
    node->identifier("_main");
  else if (node->identifier() == "_main")
    node->identifier("._main");

  auto symbol = symbol::make_symbol(node->type(), node->identifier(), true, node->qualifier(), node->arguments());

  auto existent_symbol = _symtab.find(node->identifier());

  if (existent_symbol == nullptr) {
    _symtab.insert(node->identifier(), symbol);
    _parent->set_new_symbol(symbol);
    return;
  }

  //IF already exists
  if (node->arguments()->size() == existent_symbol->params()->size()) {
    for (size_t i=0; i < node->arguments()->size(); i++) {
      cdk::typed_node* newArgument = (cdk::typed_node*) node->arguments()->node(i);
      if (newArgument->type()->name() != existent_symbol->params()->at(i)->type()->name())
        throw std::string("declarations are not the same");
    }

    _symtab.replace(node->identifier(), symbol);
    _parent->set_new_symbol(symbol);
  }
  else throw std::string("declarations are not the same");
}

void fir::type_checker::do_null_pointer_node(fir::null_pointer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4,cdk::primitive_type::create(0, cdk::TYPE_VOID)));
}

void fir::type_checker::do_size_of_node(fir::size_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->statement()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_address_of_node(fir::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
}

void fir::type_checker::do_alloc_node(fir::alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("Wrong type.");
  }

  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}
