#ifndef __FIR_AST_BRACKET_NODE_H__
#define __FIR_AST_BRACKET_NODE_H__


#include <cdk/ast/sequence_node.h>

namespace fir {

  /**
   * Class for describing brackets nodes.
   */
  class bracket_node: public cdk::basic_node {
    cdk::sequence_node *_expressions;

    public:
    bracket_node(int lineno, cdk::sequence_node *expressions):
        cdk::basic_node(lineno), _expressions(expressions){
        }
    public:
        cdk::sequence_node *expressions(){
            return _expressions;
        }
        void accept(basic_ast_visitor *sp, int level) {
        sp->do_bracket_node(this, level);
        }
    };
} //fir

#endif


