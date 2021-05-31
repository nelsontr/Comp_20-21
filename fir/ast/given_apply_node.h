#ifndef __FIR_AST_GIVEN_APPLY_NODE_H__
#define __FIR_AST_GIVEN_APPLY_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir{

    /**
   * Class for describing while-finally-cycle nodes.
   */
  class given_apply_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    std::string _identifier;
    std::string _pointer;
    int _iterations;

  public:
    inline given_apply_node(int lineno, cdk::expression_node *condition, const std::string &identifier, std::string &pointer, const int &iterations) :
        basic_node(lineno), _condition(condition), _identifier(identifier), _pointer(pointer), _iterations(iterations) {
            
    }

  public:
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline std::string identifier() {
      return _identifier;
    }

    inline std::string pointer(){
        return _pointer;
    }

    int iterations(){
        return _iterations;
    }

    void accept(basic_ast_visitor *sp, int level) {
     sp->do_given_apply_node(this, level);
    }

  };

} // fir

#endif
