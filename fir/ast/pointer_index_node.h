#ifndef __FIR_AST_POINTER_INDEX_NODE_H__
#define __FIR_AST_POINTER_INDEX_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace fir {

  class pointer_index_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;

  public:
    inline pointer_index_node(int lineno, cdk::lvalue_node *argument) :
        cdk::expression_node(lineno), _lvalue(argument) {
    }

  public:
    inline cdk::lvalue_node *lvalue() {
      return _lvalue;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_pointer_index_node(this, level);
    }

  };

}

#endif