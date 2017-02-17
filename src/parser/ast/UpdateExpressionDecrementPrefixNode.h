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

#ifndef UpdateExpressionDecrementPrefixNode_h
#define UpdateExpressionDecrementPrefixNode_h

#include "ExpressionNode.h"

namespace Escargot {

class UpdateExpressionDecrementPrefixNode : public ExpressionNode {
public:
    friend class ScriptParser;

    UpdateExpressionDecrementPrefixNode(Node* argument)
        : ExpressionNode()
    {
        m_argument = (ExpressionNode*)argument;
    }
    virtual ~UpdateExpressionDecrementPrefixNode()
    {
        delete m_argument;
    }

    virtual ASTNodeType type() { return ASTNodeType::UpdateExpressionDecrementPrefix; }
    virtual void generateExpressionByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context)
    {
        size_t resultRegisterExpect = context->getRegister();
        context->giveUpRegister();

        m_argument->generateResolveAddressByteCode(codeBlock, context);
        m_argument->generateReferenceResolvedAddressByteCode(codeBlock, context);
        size_t srcIndex = context->getLastRegisterIndex();
        context->giveUpRegister();
        size_t dstIndex = context->getRegister();
        codeBlock->pushCode(ToNumber(ByteCodeLOC(m_loc.index), srcIndex, dstIndex), context, this);
        size_t resultRegisterIndex = context->getLastRegisterIndex();
        size_t literalRegisterIndex = context->getRegister();
        codeBlock->pushCode(LoadLiteral(ByteCodeLOC(m_loc.index), literalRegisterIndex, Value(-1)), context, this);
        codeBlock->pushCode(BinaryPlus(ByteCodeLOC(m_loc.index), resultRegisterIndex, literalRegisterIndex, resultRegisterIndex), context, this);
        context->giveUpRegister();
        m_argument->generateStoreByteCode(codeBlock, context, false);
        if (resultRegisterExpect != context->getLastRegisterIndex()) {
            codeBlock->pushCode(Move(ByteCodeLOC(m_loc.index), context->getLastRegisterIndex(), resultRegisterExpect), context, this);
            context->giveUpRegister();
            context->getRegister();
            ASSERT(context->getLastRegisterIndex() == resultRegisterExpect);
        }
    }

protected:
    ExpressionNode* m_argument;
};
}

#endif
