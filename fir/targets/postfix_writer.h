#ifndef __FIR_TARGETS_POSTFIX_WRITER_H__
#define __FIR_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <stack>
#include <sstream>
#include <set>
#include <cdk/emitters/basic_postfix_emitter.h>

#define INT_POINTER ( node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_POINTER) )
#define INT_DOUBLE_STRING ( node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_STRING) )
#define INT_DOUBLE_POINTER ( node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER) )
#define INT_STRING_POINTER ( node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER) )
#define INT_DOUBLE_POINTER_STRING ( node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)  || node->is_typed(cdk::TYPE_STRING) )


namespace fir {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<fir::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;
    std::shared_ptr<fir::symbol> _function;
    int _return_count;
    int _offset;
    bool _insideFunction = false;
    bool _insideFunctionArgs = false;
    std::stack<int> _whileEnd;
    std::stack<int> _whileCondition;
    std::set<std::string> public_symbols;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<fir::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

  void public_symbol(std::string identifier){
    public_symbols.insert(identifier);
  }

  void private_symbol(std::string identifier){
    public_symbols.erase(identifier);
  }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end
  };

} // fir

#endif
