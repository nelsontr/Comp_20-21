#ifndef __FIR_AST_ALLOC_NODE_H__
#define __FIR_AST_ALLOC_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace fir {

  /**
   * Class for describing memory allocation nodes.
   */
  class alloc_node: public cdk::unary_operation_node {
  public:
    inline alloc_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_operation_node(lineno, argument) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_alloc_node(this, level);
    }

  };

} // fir

#endif
