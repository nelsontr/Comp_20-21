#ifndef __FIR_AST_BLOCK_NODE_H__
#define __FIR_AST_BLOCK_NODE_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing block nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::basic_node *_declarations, *_instructions;
    bool _epilogue;

  public:
    inline block_node(int lineno, cdk::basic_node *declarations, cdk::basic_node *instructions, bool epilogue = false) :
        cdk::basic_node(lineno), _declarations(declarations), _instructions(instructions), _epilogue(epilogue){
    }

  public:
    inline cdk::basic_node *declarations() {
      return _declarations;
    }
    inline cdk::basic_node *instructions() {
      return _instructions;
    }

    bool getEpilogue epilogue(){
      return _epilogue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // fir

#endif