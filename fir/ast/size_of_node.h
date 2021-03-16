#ifndef __FIR_AST_SIZE_OF_NODE_H__
#define __FIR_AST_SIZE_OF_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing size of nodes.
   */
  class size_of_node: public cdk::expression_node {
    cdk::expression_node *_statement;

  public:
    inline size_of_node(int lineno, cdk::expression_node *statement) :
        cdk::expression_node(lineno), _statement(statement) {
    }

  public:
    inline cdk::expression_node *statement() {
      return _statement;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_size_of_node(this, level);
    }

  };

} // fir

#endif