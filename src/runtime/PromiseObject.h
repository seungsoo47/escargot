#ifndef __EscargotPromiseObject__
#define __EscargotPromiseObject__

#if ESCARGOT_ENABLE_PROMISE

#include "runtime/Object.h"

namespace Escargot {

struct PromiseReaction {
public:
    struct Capability {
    public:
        Capability()
            : m_promise()
            , m_resolveFunction(nullptr)
            , m_rejectFunction(nullptr)
        {
        }

        Capability(Value promise, FunctionObject* resolveFunction, FunctionObject* rejectFunction)
            : m_promise(promise)
            , m_resolveFunction(resolveFunction)
            , m_rejectFunction(rejectFunction)
        {
        }

        Capability(const Capability& other)
            : m_promise(other.m_promise)
            , m_resolveFunction(other.m_resolveFunction)
            , m_rejectFunction(other.m_rejectFunction)
        {
        }

        Value m_promise;
        FunctionObject* m_resolveFunction;
        FunctionObject* m_rejectFunction;
    };

    PromiseReaction()
        : m_capability()
        , m_handler(nullptr)
    {
    }

    PromiseReaction(FunctionObject* handler, const Capability& capability)
        : m_capability(capability)
        , m_handler(handler)
    {
    }


    Capability m_capability;
    FunctionObject* m_handler;
};

class PromiseObject : public Object {
public:
    enum PromiseState {
        Pending,
        FulFilled,
        Rejected
    };

    PromiseObject(ExecutionState& state);

    virtual bool isPromiseObject()
    {
        return true;
    }

    // http://www.ecma-international.org/ecma-262/5.1/#sec-8.6.2
    virtual const char* internalClassProperty()
    {
        return "Promise";
    }

    void fulfillPromise(ExecutionState& state, Value value);
    void rejectPromise(ExecutionState& state, Value reason);

    typedef std::vector<PromiseReaction, gc_allocator_ignore_off_page<PromiseReaction> > Reactions;
    void triggerPromiseReactions(ExecutionState& state, Reactions& reactions);

    void appendReaction(FunctionObject* onFulfilled, FunctionObject* onRejected, PromiseReaction::Capability& capability)
    {
        m_fulfillReactions.push_back(PromiseReaction(onFulfilled, capability));
        m_rejectReactions.push_back(PromiseReaction(onRejected, capability));
    }

    PromiseReaction::Capability createResolvingFunctions(ExecutionState& state);
    static PromiseReaction::Capability newPromiseCapability(ExecutionState& state, Object* constructor);

    static Object* resolvingFunctionAlreadyResolved(ExecutionState& state, FunctionObject* callee);

    PromiseState state() { return m_state; }
    Value promiseResult()
    {
        ASSERT(m_state != PromiseState::Pending);
        return m_promiseResult;
    }

protected:
    PromiseState m_state;
    Value m_promiseResult;
    Reactions m_fulfillReactions;
    Reactions m_rejectReactions;

protected:
};

Value getCapabilitiesExecutorFunction(ExecutionState& state, Value thisValue, size_t argc, Value* argv, bool isNewExpression);
Value promiseResolveFunction(ExecutionState& state, Value thisValue, size_t argc, Value* argv, bool isNewExpression);
Value promiseRejectFunction(ExecutionState& state, Value thisValue, size_t argc, Value* argv, bool isNewExpression);
Value promiseAllResolveElementFunction(ExecutionState& state, Value thisValue, size_t argc, Value* argv, bool isNewExpression);
}

#endif // ESCARGOT_ENABLE_PROMISE

#endif // __EscargotPromiseObject__