/*
 * Copyright (c) 2016-present Samsung Electronics Co., Ltd
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 */

#ifndef __EscargotObjectGlobalObject__
#define __EscargotObjectGlobalObject__

#include "runtime/FunctionObject.h"
#include "runtime/Object.h"

namespace Escargot {

class FunctionObject;

#define RESOLVE_THIS_BINDING_TO_OBJECT(NAME, OBJ, BUILT_IN_METHOD)                                                                                                                                                                              \
    if (thisValue.isUndefinedOrNull()) {                                                                                                                                                                                                        \
        ErrorObject::throwBuiltinError(state, ErrorObject::TypeError, state.context()->staticStrings().OBJ.string(), true, state.context()->staticStrings().BUILT_IN_METHOD.string(), ErrorObject::Messages::GlobalObject_ThisUndefinedOrNull); \
    }                                                                                                                                                                                                                                           \
    Object* NAME = thisValue.toObject(state);

#define RESOLVE_THIS_BINDING_TO_STRING(NAME, OBJ, BUILT_IN_METHOD)                                                                                                                                                                              \
    if (thisValue.isUndefinedOrNull()) {                                                                                                                                                                                                        \
        ErrorObject::throwBuiltinError(state, ErrorObject::TypeError, state.context()->staticStrings().OBJ.string(), true, state.context()->staticStrings().BUILT_IN_METHOD.string(), ErrorObject::Messages::GlobalObject_ThisUndefinedOrNull); \
    }                                                                                                                                                                                                                                           \
    String* NAME = thisValue.toString(state);

Value builtinSpeciesGetter(ExecutionState& state, Value thisValue, size_t argc, Value* argv, Optional<Object*> newTarget);

class GlobalObject : public Object {
public:
    friend class ByteCodeInterpreter;
    friend class GlobalEnvironmentRecord;
    friend class IdentifierNode;

    explicit GlobalObject(ExecutionState& state);

    virtual bool isGlobalObject() const override
    {
        return true;
    }

    void installBuiltins(ExecutionState& state);

    Value eval(ExecutionState& state, const Value& arg);
    Value evalLocal(ExecutionState& state, const Value& arg, Value thisValue, InterpretedCodeBlock* parentCodeBlock, bool inWithOperation); // we get isInWithOperation as parameter because this affects bytecode

    FunctionObject* object()
    {
        return m_object;
    }
    Object* objectPrototype()
    {
        return m_objectPrototype;
    }
    FunctionObject* objectPrototypeToString()
    {
        return m_objectPrototypeToString;
    }
    FunctionObject* objectCreate()
    {
        return m_objectCreate;
    }
    FunctionObject* objectFreeze()
    {
        return m_objectFreeze;
    }
    FunctionObject* function()
    {
        return m_function;
    }
    FunctionObject* functionPrototype()
    {
        return m_functionPrototype;
    }
    FunctionObject* functionApply()
    {
        return m_functionApply;
    }

    FunctionObject* error()
    {
        return m_error;
    }
    Object* errorPrototype()
    {
        return m_errorPrototype;
    }
    FunctionObject* referenceError()
    {
        return m_referenceError;
    }
    Object* referenceErrorPrototype()
    {
        return m_referenceErrorPrototype;
    }
    FunctionObject* typeError()
    {
        return m_typeError;
    }
    Object* typeErrorPrototype()
    {
        return m_typeErrorPrototype;
    }
    FunctionObject* rangeError()
    {
        return m_rangeError;
    }
    Object* rangeErrorPrototype()
    {
        return m_rangeErrorPrototype;
    }
    FunctionObject* syntaxError()
    {
        return m_syntaxError;
    }
    Object* syntaxErrorPrototype()
    {
        return m_syntaxErrorPrototype;
    }
    FunctionObject* uriError()
    {
        return m_uriError;
    }
    Object* uriErrorPrototype()
    {
        return m_uriErrorPrototype;
    }
    FunctionObject* evalError()
    {
        return m_evalError;
    }
    Object* evalErrorPrototype()
    {
        return m_evalErrorPrototype;
    }

    FunctionObject* string()
    {
        return m_string;
    }
    Object* stringPrototype()
    {
        return m_stringPrototype;
    }
    Object* stringIteratorPrototype()
    {
        return m_stringIteratorPrototype;
    }

    FunctionObject* number()
    {
        return m_number;
    }
    Object* numberPrototype()
    {
        return m_numberPrototype;
    }

    FunctionObject* symbol()
    {
        return m_symbol;
    }
    Object* symbolPrototype()
    {
        return m_symbolPrototype;
    }

    FunctionObject* array()
    {
        return m_array;
    }
    Object* arrayPrototype()
    {
        return m_arrayPrototype;
    }
    Object* arrayIteratorPrototype()
    {
        return m_arrayIteratorPrototype;
    }

    FunctionObject* arrayPrototypeValues()
    {
        return m_arrayPrototypeValues;
    }

    FunctionObject* boolean()
    {
        return m_boolean;
    }
    Object* booleanPrototype()
    {
        return m_booleanPrototype;
    }

    FunctionObject* date()
    {
        return m_date;
    }
    Object* datePrototype()
    {
        return m_datePrototype;
    }

    Object* math()
    {
        return m_math;
    }

    FunctionObject* regexp()
    {
        return m_regexp;
    }

    Object* regexpPrototype()
    {
        return m_regexpPrototype;
    }

    FunctionObject* regexpSplitMethod()
    {
        return m_regexpSplitMethod;
    }
    FunctionObject* regexpReplaceMethod()
    {
        return m_regexpReplaceMethod;
    }
    FunctionObject* regexpExecMethod()
    {
        return m_regexpExecMethod;
    }
    Object* json()
    {
        return m_json;
    }

    FunctionObject* jsonStringify()
    {
        return m_jsonStringify;
    }

    FunctionObject* jsonParse()
    {
        return m_jsonParse;
    }
#if defined(ENABLE_ICU) && defined(ENABLE_INTL)
    Object* intl()
    {
        return m_intl;
    }

    FunctionObject* intlCollator()
    {
        return m_intlCollator;
    }

    const Vector<String*, GCUtil::gc_malloc_allocator<String*>>& intlCollatorAvailableLocales();

    FunctionObject* intlDateTimeFormat()
    {
        return m_intlDateTimeFormat;
    }

    const Vector<String*, GCUtil::gc_malloc_allocator<String*>>& intlDateTimeFormatAvailableLocales();

    FunctionObject* intlNumberFormat()
    {
        return m_intlNumberFormat;
    }

    const Vector<String*, GCUtil::gc_malloc_allocator<String*>>& intlNumberFormatAvailableLocales();

    FunctionObject* intlLocale()
    {
        return m_intlLocale;
    }

    Object* intlLocalePrototype()
    {
        return m_intlLocalePrototype;
    }
#endif
    FunctionObject* promise()
    {
        return m_promise;
    }
    Object* promisePrototype()
    {
        return m_promisePrototype;
    }
    FunctionObject* proxy()
    {
        return m_proxy;
    }
    FunctionObject* arrayBuffer()
    {
        return m_arrayBuffer;
    }
    Object* arrayBufferPrototype()
    {
        return m_arrayBufferPrototype;
    }
    FunctionObject* dataView()
    {
        return m_dataView;
    }
    Object* dataViewPrototype()
    {
        return m_dataViewPrototype;
    }
    Object* typedArray()
    {
        return m_typedArray;
    }
    Object* typedArrayPrototype()
    {
        return m_typedArrayPrototype;
    }
    Object* int8Array()
    {
        return m_int8Array;
    }
    Object* int8ArrayPrototype()
    {
        return m_int8ArrayPrototype;
    }
    Object* uint8Array()
    {
        return m_uint8Array;
    }
    Object* uint8ArrayPrototype()
    {
        return m_uint8ArrayPrototype;
    }
    Object* int16Array()
    {
        return m_int16Array;
    }
    Object* int16ArrayPrototype()
    {
        return m_int16ArrayPrototype;
    }
    Object* uint16Array()
    {
        return m_uint16Array;
    }
    Object* uint16ArrayPrototype()
    {
        return m_uint16ArrayPrototype;
    }
    Object* int32Array()
    {
        return m_int32Array;
    }
    Object* int32ArrayPrototype()
    {
        return m_int32ArrayPrototype;
    }
    Object* uint32Array()
    {
        return m_uint32Array;
    }
    Object* uint32ArrayPrototype()
    {
        return m_uint32ArrayPrototype;
    }
    Object* uint8ClampedArray()
    {
        return m_uint8ClampedArray;
    }
    Object* uint8ClampedArrayPrototype()
    {
        return m_uint8ClampedArrayPrototype;
    }
    Object* float32Array()
    {
        return m_float32Array;
    }
    Object* float32ArrayPrototype()
    {
        return m_float32ArrayPrototype;
    }
    Object* float64Array()
    {
        return m_float64Array;
    }
    Object* float64ArrayPrototype()
    {
        return m_float64ArrayPrototype;
    }

    FunctionObject* map()
    {
        return m_map;
    }

    Object* mapPrototype()
    {
        return m_mapPrototype;
    }

    Object* mapIteratorPrototype()
    {
        return m_mapIteratorPrototype;
    }

    FunctionObject* set()
    {
        return m_set;
    }

    Object* setPrototype()
    {
        return m_setPrototype;
    }

    Object* setIteratorPrototype()
    {
        return m_setIteratorPrototype;
    }

    FunctionObject* weakMap()
    {
        return m_weakMap;
    }

    Object* weakMapPrototype()
    {
        return m_weakMapPrototype;
    }

    FunctionObject* weakSet()
    {
        return m_weakSet;
    }

    Object* weakSetPrototype()
    {
        return m_weakSetPrototype;
    }

    FunctionObject* generatorFunction()
    {
        return m_generatorFunction;
    }

    FunctionObject* generator()
    {
        return m_generator;
    }

    Object* generatorPrototype()
    {
        return m_generatorPrototype;
    }

    FunctionObject* asyncFunction()
    {
        return m_asyncFunction;
    }

    Object* asyncFunctionPrototype()
    {
        return m_asyncFunctionPrototype;
    }

    Object* asyncIteratorPrototype()
    {
        return m_asyncIteratorPrototype;
    }

    Object* asyncFromSyncIteratorPrototype()
    {
        return m_asyncFromSyncIteratorPrototype;
    }

    Object* asyncGenerator()
    {
        return m_asyncGenerator;
    }

    Object* asyncGeneratorPrototype()
    {
        return m_asyncGeneratorPrototype;
    }

    FunctionObject* asyncGeneratorFunction()
    {
        return m_asyncGeneratorFunction;
    }

    FunctionObject* eval()
    {
        return m_eval;
    }

    // http://www.ecma-international.org/ecma-262/5.1/#sec-13.2.3
    // 13.2.3 The [[ThrowTypeError]] Function Object
    FunctionObject* throwTypeError()
    {
        ASSERT(m_throwTypeError);
        return m_throwTypeError;
    }

    JSGetterSetter* throwerGetterSetterData()
    {
        ASSERT(m_throwerGetterSetterData);
        return m_throwerGetterSetterData;
    }

    StringObject* stringProxyObject()
    {
        return m_stringProxyObject;
    }

    NumberObject* numberProxyObject()
    {
        return m_numberProxyObject;
    }

    BooleanObject* booleanProxyObject()
    {
        return m_booleanProxyObject;
    }

    SymbolObject* symbolProxyObject()
    {
        return m_symbolProxyObject;
    }

    virtual bool isInlineCacheable() override
    {
        return false;
    }

    virtual ObjectHasPropertyResult hasProperty(ExecutionState& state, const ObjectPropertyName& P) override ESCARGOT_OBJECT_SUBCLASS_MUST_REDEFINE;
    virtual ObjectGetResult getOwnProperty(ExecutionState& state, const ObjectPropertyName& P) override ESCARGOT_OBJECT_SUBCLASS_MUST_REDEFINE;

    void* operator new(size_t size)
    {
        return GC_MALLOC(size);
    }
    void* operator new[](size_t size) = delete;

private:
    Context* m_context;

    FunctionObject* m_object;
    Object* m_objectPrototype;
    FunctionObject* m_objectPrototypeToString;
    FunctionObject* m_objectCreate;
    FunctionObject* m_objectFreeze;

    FunctionObject* m_function;
    FunctionObject* m_functionPrototype;
    FunctionObject* m_functionApply;

    Object* m_iteratorPrototype;

    FunctionObject* m_error;
    Object* m_errorPrototype;
    FunctionObject* m_referenceError;
    Object* m_referenceErrorPrototype;
    FunctionObject* m_typeError;
    Object* m_typeErrorPrototype;
    FunctionObject* m_rangeError;
    Object* m_rangeErrorPrototype;
    FunctionObject* m_syntaxError;
    Object* m_syntaxErrorPrototype;
    FunctionObject* m_uriError;
    Object* m_uriErrorPrototype;
    FunctionObject* m_evalError;
    Object* m_evalErrorPrototype;

    FunctionObject* m_string;
    Object* m_stringPrototype;
    Object* m_stringIteratorPrototype;

    FunctionObject* m_number;
    Object* m_numberPrototype;

    FunctionObject* m_symbol;
    Object* m_symbolPrototype;

    FunctionObject* m_array;
    Object* m_arrayPrototype;
    Object* m_arrayIteratorPrototype;
    // https://www.ecma-international.org/ecma-262/6.0/#sec-well-known-intrinsic-objects
    // Well-Known Intrinsic Objects : %ArrayProto_values%
    // The initial value of the values data property of %ArrayPrototype%
    FunctionObject* m_arrayPrototypeValues;

    FunctionObject* m_boolean;
    Object* m_booleanPrototype;

    FunctionObject* m_date;
    Object* m_datePrototype;

    FunctionObject* m_regexp;
    Object* m_regexpPrototype;
    FunctionObject* m_regexpSplitMethod;
    FunctionObject* m_regexpReplaceMethod;
    FunctionObject* m_regexpExecMethod;

    Object* m_math;

    FunctionObject* m_eval;

    FunctionObject* m_throwTypeError;
    JSGetterSetter* m_throwerGetterSetterData;

    StringObject* m_stringProxyObject;
    NumberObject* m_numberProxyObject;
    BooleanObject* m_booleanProxyObject;
    SymbolObject* m_symbolProxyObject;

    Object* m_json;
    FunctionObject* m_jsonStringify;
    FunctionObject* m_jsonParse;
#if defined(ENABLE_ICU) && defined(ENABLE_INTL)
    Object* m_intl;
    FunctionObject* m_intlCollator;
    Vector<String*, GCUtil::gc_malloc_allocator<String*>> m_intlCollatorAvailableLocales;
    FunctionObject* m_intlDateTimeFormat;
    Vector<String*, GCUtil::gc_malloc_allocator<String*>> m_intlDateTimeFormatAvailableLocales;
    FunctionObject* m_intlNumberFormat;
    Vector<String*, GCUtil::gc_malloc_allocator<String*>> m_intlNumberFormatAvailableLocales;
    FunctionObject* m_intlLocale; // %Locale%
    Object* m_intlLocalePrototype; // %LocalePrototype%
#endif

    FunctionObject* m_promise;
    Object* m_promisePrototype;

    FunctionObject* m_proxy;

    Object* m_reflect;

    FunctionObject* m_arrayBuffer;
    Object* m_arrayBufferPrototype;
    FunctionObject* m_dataView;
    Object* m_dataViewPrototype;
    FunctionObject* m_typedArray;
    Object* m_typedArrayPrototype;
    FunctionObject* m_int8Array;
    Object* m_int8ArrayPrototype;
    FunctionObject* m_uint8Array;
    Object* m_uint8ArrayPrototype;
    FunctionObject* m_uint8ClampedArray;
    Object* m_uint8ClampedArrayPrototype;
    FunctionObject* m_int16Array;
    Object* m_int16ArrayPrototype;
    FunctionObject* m_uint16Array;
    Object* m_uint16ArrayPrototype;
    FunctionObject* m_int32Array;
    Object* m_int32ArrayPrototype;
    FunctionObject* m_uint32Array;
    Object* m_uint32ArrayPrototype;
    FunctionObject* m_float32Array;
    Object* m_float32ArrayPrototype;
    FunctionObject* m_float64Array;
    Object* m_float64ArrayPrototype;

    FunctionObject* m_map;
    Object* m_mapPrototype;
    Object* m_mapIteratorPrototype;
    FunctionObject* m_set;
    Object* m_setPrototype;
    Object* m_setIteratorPrototype;
    FunctionObject* m_weakMap;
    Object* m_weakMapPrototype;
    FunctionObject* m_weakSet;
    Object* m_weakSetPrototype;

    FunctionObject* m_generatorFunction; // %GeneratorFunction%
    FunctionObject* m_generator; // %Generator%
    Object* m_generatorPrototype; // %GeneratorPrototype%

    FunctionObject* m_asyncFunction; // %AsyncFunction%
    Object* m_asyncFunctionPrototype; // %AsyncFunctionPrototype%

    Object* m_asyncIteratorPrototype; // %AsyncIteratorPrototype%
    Object* m_asyncFromSyncIteratorPrototype; // %AsyncFromSyncIteratorPrototype%

    Object* m_asyncGenerator; // %AsyncGenerator%
    Object* m_asyncGeneratorPrototype; // %AsyncGeneratorPrototype%
    FunctionObject* m_asyncGeneratorFunction; // %AsyncGeneratorFunction%

    void installFunction(ExecutionState& state);
    void installObject(ExecutionState& state);
    void installError(ExecutionState& state);
    void installSymbol(ExecutionState& state);
    void installString(ExecutionState& state);
    void installNumber(ExecutionState& state);
    void installBoolean(ExecutionState& state);
    void installArray(ExecutionState& state);
    void installMath(ExecutionState& state);
    void installDate(ExecutionState& state);
    void installRegExp(ExecutionState& state);
    void installJSON(ExecutionState& state);
#if defined(ENABLE_ICU) && defined(ENABLE_INTL)
    void installIntl(ExecutionState& state);
#endif
    void installPromise(ExecutionState& state);
    void installProxy(ExecutionState& state);
    void installReflect(ExecutionState& state);
    void installDataView(ExecutionState& state);
    void installTypedArray(ExecutionState& state);
    template <typename TA, int elementSize, typename TypeAdaptor>
    FunctionObject* installTypedArray(ExecutionState& state, AtomicString taName, Object** proto, FunctionObject* typedArrayFunction);
    void installIterator(ExecutionState& state);
    void installMap(ExecutionState& state);
    void installSet(ExecutionState& state);
    void installWeakMap(ExecutionState& state);
    void installWeakSet(ExecutionState& state);
    void installGenerator(ExecutionState& state);
    void installAsyncFunction(ExecutionState& state);
    void installAsyncIterator(ExecutionState& state);
    void installAsyncFromSyncIterator(ExecutionState& state);
    void installAsyncGeneratorFunction(ExecutionState& state);
    void installOthers(ExecutionState& state);
};
}

#endif
