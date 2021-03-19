#ifndef __FIR_AST_DECLARATION_VARAIABLE_NODE_H__
#define __FIR_AST_DECLARATION_VARAIABLE_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/typed_node.h>

namespace fir {

  /**
   * Class for describing declaration variable nodes.
   */
  class block_node: public cdk::typed_node {
    int _qualifier;
    std::string _variableId;
    cdk::expression_node *_initializer;

    public:
    variable_declaration_node(int lineno, int qualifier, cdk::basic_type *type, std::string &variableId,
                              cdk::expression_node *initializer):
        cdk::typed_node(lineno), _qualifier(qualifier), _variableId(variableId), _initializer(initializer) {
    (this)->type(std::shared_ptr<cdk::basic_type>(type));
    }
    public:
    
    int qualifier() {
      return _qualifier;
    }
    std::string &variableId() {
      return _variableId;
    }
    cdk::expression_node *initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_variable_node(this, level);
    }

  };

}
#endif
