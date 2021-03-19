#ifndef __FIR_AST_IDENTIFY_NODE_H__
#define __FIR_AST_IDENTIFY_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace fir {

  /**
   * Class for describing the identity operators OPERATION nodes.
   */
  class identify_node: public cdk::unary_operation_node {
    bool _signal; 

  public:
    inline identify_node(int lineno, cdk::expression_node *arg, bool signal=false):
        cdk::unary_operation_node(lineno, arg), _signal(signal){
    }

    public:
    inline bool signal() {
      return _signal;
    } 


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identify_node(this, level);
    }

  };

} // fir

#endif