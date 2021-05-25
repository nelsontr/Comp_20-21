#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    bool _function;
    int _acessType; //Public=1, Private=2, Import=0
    std::vector<std::shared_ptr<symbol>> _params; //Functions parameters
    int _offset;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, bool function, int acessType) :
        _type(type), _name(name), _function(function), _acessType(acessType), _offset(0) {
    }

    virtual ~symbol() {
      // EMPTY
    }
    int offset() const {
      return _offset;
    }
    int offset(int offset) {
      return _offset = offset;
    }
    bool function() const {
      return _function;
    }
    bool function(bool function) {
      return _function = function;
    }
    int acessType() const {
      return _acessType;
    }
    int acessType(int acessType) {
      return _acessType = acessType;
    }
    std::vector<std::shared_ptr<symbol>> *params(){
      return &_params;
    }
    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
  };

} // fir

#endif
