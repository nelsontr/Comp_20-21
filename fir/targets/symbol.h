#ifndef FIR_TARGETS_SYMBOL_H
#define FIR_TARGETS_SYMBOL_H

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir
{

  class symbol
  {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    bool _function;
    int _acessType; //Public=1, Private=2, Import=0
    int _offset;

    size_t _argument_size;
    bool _isDefined = false;
    std::vector<std::shared_ptr<symbol>> _params; //Functions parameters
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, bool function, int acessType) : _type(type), _name(name), _function(function), _acessType(acessType), _offset(0)
    {
    }

    virtual ~symbol()
    {
      // EMPTY
    }
    int offset() const
    {
      return _offset;
    }
    int offset(int offset)
    {
      return _offset = offset;
    }
    bool function() const
    {
      return _function;
    }
    bool function(bool function)
    {
      return _function = function;
    }

    bool isDefined() const
    {
      return _isDefined;
    }

    bool isDefined(bool isDefined)
    {
      return _isDefined = isDefined;
    }

    int acessType() const
    {
      return _acessType;
    }

    int acessType(int acessType)
    {
      return _acessType = acessType;
    }

    std::vector<std::shared_ptr<symbol>> *params()
    {
      return &_params;
    }

    std::shared_ptr<cdk::basic_type> type() const
    {
      return _type;
    }

    bool is_typed(cdk::typename_type name) const
    {
      return _type->name() == name;
    }

    const std::string &name() const
    {
      return _name;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types)
    {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const
    {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const
    {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const
    {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const
    {
      return _argument_types.size();
    }
  };
} // fir

#endif