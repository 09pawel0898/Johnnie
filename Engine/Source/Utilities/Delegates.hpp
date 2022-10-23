#pragma once

#include <utility>
#include <vector>
#include <variant>
#include <memory>

#include "Core/CoreMinimal.hpp"

template <typename Signature>
class CallableWrapper;

template <typename TRetVal, typename... Args>
class CallableWrapper<TRetVal(Args...)>
{
public:
    virtual ~CallableWrapper() = default;

    virtual TRetVal Execute(Args&&... Args_) = 0;
};

template <typename Signature>
class CallableStaticFunctionWrapper;

template <typename TRetVal, typename... Args>
class CallableStaticFunctionWrapper<TRetVal(Args...)> final : public CallableWrapper<TRetVal(Args...)>
{
private:
    TRetVal(*m_StaticFuncPtr)(Args...) { nullptr };

public:
    explicit CallableStaticFunctionWrapper(TRetVal(*StaticFuncPtr)(Args...)) noexcept
        :   m_StaticFuncPtr(StaticFuncPtr)
    {}

    virtual void Execute(Args&&... Args_) override final
    {
        if (m_StaticFuncPtr)
        {
            m_StaticFuncPtr(Forward<Args>(Args_)...);
        }
    }
};

template <bool IsConst, typename TClass, typename Signature>
class CallableMethodWrapper;

template <bool IsConst, typename TClass, typename TRetVal, typename... Args>
class CallableMethodWrapper<IsConst, TClass, TRetVal(Args...)> final : public CallableWrapper<TRetVal(Args...)>
{
private:
    template<bool Const>
    struct MemberFunction;

    template <> struct MemberFunction<true>
    {
        using Type = TRetVal(TClass::*)(Args...) const;
    };

    template <> struct MemberFunction<false>
    {
        using Type = TRetVal(TClass::*)(Args...);
    };

    using ClassMemberFunction = MemberFunction<IsConst>::Type;
    
    TClass* m_Instance{ nullptr };
    ClassMemberFunction m_ClassMemFuncPtr { nullptr };

public:
    CallableMethodWrapper(TClass* Instance, ClassMemberFunction ClassMemFuncPtr) noexcept
        :   m_Instance(Instance),  
            m_ClassMemFuncPtr(ClassMemFuncPtr) 
    {}

    virtual void Execute(Args&&... Args_) override
    { 
        if (m_Instance != nullptr)
        {
            (m_Instance->*m_ClassMemFuncPtr)(Forward<Args>(Args_)...);
        }
    }
};

template <typename TLambda, typename Signature>
class CallableLambdaWrapper;

template <typename TLambda, typename TRetVal, typename... Args>
class CallableLambdaWrapper<TLambda, TRetVal(Args...)> final : public CallableWrapper<TRetVal(Args...)>
{
private:
    TLambda m_LambdaObject;

public:
    explicit CallableLambdaWrapper(TLambda&& FunctorObject) noexcept
        : m_LambdaObject(Forward<TLambda>(FunctorObject))
    {
        static_assert(std::is_invocable_v<TLambda,Args...>);
    }

    virtual TRetVal Execute(Args&&... Args_) override
    { 
        return m_LambdaObject(Forward<Args>(Args_)...);
    }
};

template <typename Signature>
class Delegate;

template <typename TRetVal, typename... Args>
class Delegate<TRetVal(Args...)>
{
private:
    TUniquePtr<CallableWrapper<TRetVal(Args...)>> m_CallableWrapper {nullptr};

public:
    Delegate() = default;
    ~Delegate() = default;

    Delegate(Delegate const& Rhs) = delete;

    Delegate(Delegate&& Rhs) noexcept
        :   m_CallableWrapper(MoveTemp(Rhs.m_CallableWrapper))
    {}

    Delegate& operator=(Delegate const& Rhs) = delete;

    Delegate& operator=(Delegate&& Rhs) noexcept
    {
        m_CallableWrapper = MoveTemp(Rhs.m_CallableWrapper);
        return *this;
    }

    void BindStatic(TRetVal(*StaticFuncPtr)(Args...))
    {
        m_CallableWrapper = MakeUnique<CallableStaticFunctionWrapper<TRetVal(Args...)>>(StaticFuncPtr);
    }

    template <typename TClass>
    void BindRaw(TClass* Instance, TRetVal(TClass::* ClassMemFuncPtr)(Args...))
    {
        m_CallableWrapper = MakeUnique<CallableMethodWrapper<false, TClass, TRetVal(Args...)>>(Instance, ClassMemFuncPtr);
    }

    template <typename TClass>
    void BindRaw(TClass* Instance, TRetVal(TClass::* ClassMemFuncConstPtr)(Args...) const)
    {
        m_CallableWrapper = MakeUnique<CallableMethodWrapper<true, TClass, TRetVal(Args...)>>(Instance, ClassMemFuncConstPtr);
    }

    template <typename TLambda>
    void BindLambda(TLambda&& LambdaObject)
    {
        m_CallableWrapper = MakeUnique<CallableLambdaWrapper<std::decay_t<TLambda>, TRetVal(Args...)>>(Forward<TLambda>(LambdaObject));
    }

    explicit operator bool() const 
    { 
        return m_CallableWrapper != nullptr; 
    }

    void ExecuteIfBound(Args&&... Args_)
    {
        if (m_CallableWrapper)
        {
            m_CallableWrapper->Execute(Forward<Args>(Args_)...);
        }
    }
};

template <typename Signature>
class MulticastDelegate;

template <typename TRetVal, typename... Args>
class MulticastDelegate<TRetVal(Args...)>
{
private:
    std::vector<Delegate<TRetVal(Args...)>> m_Delegates;

public:
    void AddStatic(TRetVal(*StaticFuncPtr)(Args...))
    {
        Delegate<TRetVal(Args...)> delegate_;
        delegate_.BindStatic(StaticFuncPtr);

        m_Delegates.emplace_back(MoveTemp(delegate_));
    }

    template <typename TClass>
    void AddRaw(TClass* Instance, TRetVal(TClass::* ClassMemFuncPtr)(Args...))
    {
        Delegate<TRetVal(Args...)> delegate_;
        delegate_.BindRaw(Instance, ClassMemFuncPtr);

        m_Delegates.emplace_back(MoveTemp(delegate_));
    }

    template <typename TClass>
    void AddRaw(TClass* Instance, TRetVal(TClass::* ClassMemFuncConstPtr)(Args...) const)
    {
        Delegate<TRetVal(Args...)> delegate_;
        delegate_.BindRaw(Instance, ClassMemFuncConstPtr);

        m_Delegates.emplace_back(MoveTemp(delegate_));
    }

    template <typename TLambda>
    void AddLambda(TLambda&& LambdaObject)
    {
        Delegate<TRetVal(Args...)> delegate_;
        delegate_.BindLambda(Forward<TLambda>(LambdaObject));

        m_Delegates.emplace_back(MoveTemp(delegate_));
    }

    explicit operator bool() const
    {
        return !m_Delegates.empty();
    }

    template <typename... Args>
    void Broadcast(Args&&... Args_)
    {
        for (auto& delegate_ : m_Delegates)
        {
            delegate_.ExecuteIfBound(Forward<Args>(Args_)...);
        }
    }
};

#define DECLARE_DELEGATE(DelegateName,...)\
    using DelegateName = Delegate<void(__VA_ARGS__)>;

#define DECLARE_MULTICAST_DELEGATE(DelegateName,...)\
    using DelegateName = MulticastDelegate<void(__VA_ARGS__)>;

using SimpleDelegate = Delegate<void(void)>;