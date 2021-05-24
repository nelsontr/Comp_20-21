#ifndef __FIR_AST_FUNCTION_CALL_H__
#define __FIR_AST_FUNCTION_CALL_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/nil_node.h>

namespace fir {

/**
   * Class for describing function call nodes.
   */
  class function_call_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:

    function_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments = nullptr) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }

  public:
    const std::string& identifier() {
      return _identifier;
    }
    cdk::sequence_node* arguments() {
      return _arguments;
    }
    inline void identifier(std::string identifier) {
      _identifier = identifier;
    }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // fir

#endif
