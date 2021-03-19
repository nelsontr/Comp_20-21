#ifndef __FIR_AST_NULL_POINTER_NODE_H__
#define __FIR_AST_NULL_POINTER_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing the null pointer nodes.
   */
  class null_pointer_node: public cdk::expression_node {

      
  public:
    inline null_pointer_node(int lineno):
        cdk::expression_node(lineno){
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_pointer_node(this, level);
    }

  };

} // fir

#endif