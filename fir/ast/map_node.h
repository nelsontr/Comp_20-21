#ifndef __FIR_AST_MAP_NODE_H__
#define __FIR_AST_MAP_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing map-cycle nodes.
   */
  class map_node: public cdk::basic_node {
    std::shared_ptr<cdk::basic_type> _vector;
    cdk::expression_node *_lower;
    cdk::expression_node *_higher;
    std::string _function;

  public:
    inline map_node(int lineno, std::shared_ptr<cdk::basic_type> vector, cdk::expression_node *lower, cdk::expression_node *higher, std::string const &function) :
        basic_node(lineno), _vector(vector), _lower(lower), _higher(higher), _function(function) {
    }

  public:
    inline std::shared_ptr<cdk::basic_type> vector() {
      return _vector;
    }
    inline cdk::expression_node *lower() {
      return _lower;
    }
    inline cdk::expression_node *higher() {
      return _higher;
    }
    inline std::string function(){
      return _function;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_map_node(this, level);
    }

  };

} // fir

#endif
