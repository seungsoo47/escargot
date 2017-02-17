/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef MemberExpressionNode_h
#define MemberExpressionNode_h

#include "ExpressionNode.h"
#include "IdentifierNode.h"
#include "runtime/Context.h"

namespace Escargot {

class MemberExpressionNode : public ExpressionNode {
public:
    MemberExpressionNode(Node* object, Node* property, bool computed)
        : ExpressionNode()
    {
        m_object = object;
        m_property = property;
        m_computed = computed;
    }

    virtual ~MemberExpressionNode()
    {
        delete m_object;
        delete m_property;
    }

    Node* object()
    {
        return m_object;
    }

    Node* property()
    {
        return m_property;
    }

    virtual ASTNodeType type() { return ASTNodeType::MemberExpression; }
    bool isPreComputedCase()
    {
        return m_computed;
    }

    AtomicString propertyName()
    {
        ASSERT(isPreComputedCase());
        return ((IdentifierNode*)m_property)->name();
    }

    virtual void generateExpressionByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context)
    {
        bool prevHead = context->m_isHeadOfMemberExpression;
        context->m_isHeadOfMemberExpression = false;

        size_t objectIndexExpect = context->getRegister();
        context->giveUpRegister();

        m_object->generateExpressionByteCode(codeBlock, context);
        size_t objectIndex = context->getLastRegisterIndex();

        if (context->m_inCallingExpressionScope && prevHead) {
            if (objectIndexExpect != objectIndex) {
                context->giveUpRegister();
                size_t w = context->getRegister();
                ASSERT(w == objectIndexExpect);
                codeBlock->pushCode(Move(ByteCodeLOC(m_loc.index), objectIndex, w), context, this);
            }
            size_t r1 = context->getRegister();
            ASSERT(r1 == (objectIndexExpect + 1));
            codeBlock->pushCode(Move(ByteCodeLOC(m_loc.index), objectIndex, r1), context, this);
            objectIndex = r1;
        }

        // drop objectIndex
        context->giveUpRegister();

        size_t resultIndex = context->getRegister();

        if (isPreComputedCase()) {
            ASSERT(m_property->isIdentifier());
            size_t pos = codeBlock->currentCodeSize();
            codeBlock->pushCode(GetObjectPreComputedCase(ByteCodeLOC(m_loc.index), objectIndex, resultIndex, m_property->asIdentifier()->name()), context, this);
            context->m_getObjectCodePositions.push_back(pos);
        } else {
            m_property->generateExpressionByteCode(codeBlock, context);
            size_t propertyIndex = context->getLastRegisterIndex();
            codeBlock->pushCode(GetObject(ByteCodeLOC(m_loc.index), objectIndex, propertyIndex, resultIndex), context, this);
            context->giveUpRegister();
        }
    }

    virtual void generateStoreByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context, bool needToReferenceSelf = true)
    {
        if (isPreComputedCase()) {
            size_t valueIndex;
            size_t objectIndex;
            if (needToReferenceSelf) {
                valueIndex = context->getLastRegisterIndex();
                m_object->generateExpressionByteCode(codeBlock, context);
                objectIndex = context->getLastRegisterIndex();
            } else {
                valueIndex = context->getLastRegisterIndex();
                objectIndex = context->getLastRegisterIndex(1);
            }
            SetObjectInlineCache* inlineCache = new SetObjectInlineCache();
            codeBlock->m_literalData.pushBack((PointerValue*)inlineCache);
            codeBlock->pushCode(SetObjectPreComputedCase(ByteCodeLOC(m_loc.index), objectIndex, m_property->asIdentifier()->name(), valueIndex, inlineCache), context, this);
            context->giveUpRegister();
            context->giveUpRegister();
            context->pushRegister(valueIndex);
        } else {
            size_t valueIndex;
            size_t objectIndex;
            size_t propertyIndex;
            if (needToReferenceSelf) {
                valueIndex = context->getLastRegisterIndex();
                m_object->generateExpressionByteCode(codeBlock, context);
                objectIndex = context->getLastRegisterIndex();
                m_property->generateExpressionByteCode(codeBlock, context);
                propertyIndex = context->getLastRegisterIndex();
            } else {
                valueIndex = context->getLastRegisterIndex();
                propertyIndex = context->getLastRegisterIndex(1);
                objectIndex = context->getLastRegisterIndex(2);
            }
            codeBlock->pushCode(SetObject(ByteCodeLOC(m_loc.index), objectIndex, propertyIndex, valueIndex), context, this);
            context->giveUpRegister();
            context->giveUpRegister();
            context->giveUpRegister();
            context->pushRegister(valueIndex);
        }
    }

    virtual void generateResolveAddressByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context)
    {
        m_object->generateExpressionByteCode(codeBlock, context);
        if (isPreComputedCase()) {
        } else {
            m_property->generateExpressionByteCode(codeBlock, context);
        }
    }

    virtual void generateReferenceResolvedAddressByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context)
    {
        if (isPreComputedCase()) {
            size_t objectIndex = context->getLastRegisterIndex();
            size_t resultIndex = context->getRegister();
            size_t pos = codeBlock->currentCodeSize();
            codeBlock->pushCode(GetObjectPreComputedCase(ByteCodeLOC(m_loc.index), objectIndex, resultIndex, m_property->asIdentifier()->name()), context, this);
            context->m_getObjectCodePositions.push_back(pos);
        } else {
            size_t objectIndex = context->getLastRegisterIndex(1);
            size_t propertyIndex = context->getLastRegisterIndex();
            size_t resultIndex = context->getRegister();
            codeBlock->pushCode(GetObject(ByteCodeLOC(m_loc.index), objectIndex, propertyIndex, resultIndex), context, this);
        }
    }


protected:
    Node* m_object; // object: Expression;
    Node* m_property; // property: Identifier | Expression;

    bool m_computed;
};
}

#endif
