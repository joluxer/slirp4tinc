/*
 * Callback.h
 *
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

/**
 * @class Callback
 * @brief Interface for callback functions.
 */
template<typename ReturnType, typename ...ArgTypes>
class Callback
{
public:
  //  Callback()
  //  {}

  virtual ~Callback() = default;

  virtual ReturnType execute(ArgTypes...) const =0;

  ReturnType send(ArgTypes... args) const
  {
    return execute(args...);
  }

  ReturnType notify(ArgTypes... args) const
  {
    return execute(args...);
  }
};

template<typename ReturnType>
class Callback<ReturnType>
{
public:
  //  Callback()
  //  {}

  virtual ~Callback() = default;

  virtual ReturnType execute() const =0;

  ReturnType send() const
  {
    return execute();
  }

  ReturnType notify() const
  {
    return execute();
  }
};

/**
 * @class CalleeMember
 * @brief Callback implementation for a member function of a class.
 */
template<class Class, typename ReturnType, typename ...ArgTypes>
class CalleeMember: public Callback<ReturnType, ArgTypes...>
{

public:
  typedef ReturnType
  (Class::*MemberFunctionPtr)(ArgTypes... args);

  CalleeMember(Class* _class_instance, MemberFunctionPtr _method) :
    callee(_class_instance), memberFunction(_method)
  {
  }

  ReturnType execute(ArgTypes... args) const
  {
    return (callee->*memberFunction)(args...);
  }

  ReturnType send(ArgTypes... args) const
  {
    return execute(args...);
  }

  ReturnType notify(ArgTypes... args) const
  {
    return execute(args...);
  }

private:
  Class * const callee;
  const MemberFunctionPtr memberFunction;

};

template<class Class, typename ReturnType>
class CalleeMember<Class, ReturnType> : public Callback<ReturnType>
{

public:
  typedef ReturnType
  (Class::*MemberFunctionPtr)(void);

  CalleeMember(Class* _class_instance, MemberFunctionPtr _method) :
    callee(_class_instance), memberFunction(_method)
  {
  }

  ReturnType execute() const
  {
    return (callee->*memberFunction)();
  }

  ReturnType send() const
  {
    return execute();
  }

  ReturnType notify() const
  {
    return execute();
  }

private:
  Class * const callee;
  const MemberFunctionPtr memberFunction;

};

/**
 * @class CalleeMemberWithVar
 * @brief Callback implementation for a member function of a class with additional parameter.
 *
 * @tparam Class
 * @tparam ReturnType
 * @tparam VarType
 * @tparam Parameters
 */
template<class Class, typename ReturnType, typename VarType, typename ...Parameters>
class CalleeMemberWithVar: public Callback<ReturnType, Parameters...>
{

public:
  typedef ReturnType
  (Class::*MemberFunctionPtr)(VarType, Parameters...);

  CalleeMemberWithVar(Class* _class_instance, VarType p, MemberFunctionPtr _method)
  : callee(_class_instance), param(p), memberFunction(_method)
  {}

  ReturnType execute(Parameters... args) const
  {
    return (callee->*memberFunction)(param, args...);
  }

  ReturnType send(Parameters... args) const
  {
    return execute(args...);
  }

  ReturnType notify(Parameters... args) const
  {
    return execute(args...);
  }

private:
  Class * const callee;
  VarType param;
  const MemberFunctionPtr memberFunction;
};

template<typename ReturnType, typename ...ArgTypes>
class CalleeStatic: public Callback<ReturnType, ArgTypes...>
{

public:
  typedef ReturnType
  (*FunctionPtr)(ArgTypes... args);

  CalleeStatic(FunctionPtr _func) :
    staticFunction(_func)
  {
  }

  ReturnType execute(ArgTypes... args) const
  {
    return staticFunction(args...);
  }

  ReturnType send(ArgTypes... args) const
  {
    return execute(args...);
  }

  ReturnType notify(ArgTypes... args) const
  {
    return execute(args...);
  }

private:
  const FunctionPtr staticFunction;
};

template<typename ReturnType, typename VarType, typename ...ArgTypes>
class CalleeStaticWithVar: public Callback<ReturnType, ArgTypes...>
{

public:
  typedef ReturnType
  (*FunctionPtr)(VarType, ArgTypes... args);

  CalleeStaticWithVar(VarType p, FunctionPtr _func) :
    param(p), staticFunction(_func)
  {
  }

  ReturnType execute(ArgTypes... args) const
  {
    return staticFunction(param, args...);
  }

  ReturnType send(ArgTypes... args) const
  {
    return execute(args...);
  }

  ReturnType notify(ArgTypes... args) const
  {
    return execute(args...);
  }

private:
  VarType param;
  const FunctionPtr staticFunction;
};


#endif /* CALLBACK_H_ */
