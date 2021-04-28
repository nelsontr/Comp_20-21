#ifndef __FIR_AST_RETURN_NODE_H__
#define __FIR_AST_RETURN_NODE_H__


#include <cdk/ast/sequence_node.h>

namespace fir{

    /**
   * Class for describing return nodes.
   */

  class return_node: public cdk::basic_node {
    cdk::sequence_node *_value;

    public:
    inline return_node(int lineno, cdk::sequence_node *value = nullptr) :
        cdk::basic_node(lineno), _value(value){
    }

    cdk::sequence_node *value(){
      return _value;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }
  };
} // fir

#endif