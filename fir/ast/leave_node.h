#ifndef __FIR_AST_LEAVE_NODE_H__
#define __FIR_AST_LEAVE_NODE_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing leave instruction nodes.
   */
  class leave_node: public cdk::basic_node {
      int _lvl;

  public:
    inline leave_node(int lineno, int lvl=1) :
        cdk::basic_node(lineno), _lvl(lvl) {
    }

    public:
    inline int lvl() {
      return _lvl;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_leave_node(this, level);
    }

  };

} // fir

#endif