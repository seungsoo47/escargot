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

#ifndef AssignmentExpressionModNode_h
#define AssignmentExpressionModNode_h

#include "ExpressionNode.h"
#include "IdentifierNode.h"
#include "PatternNode.h"

namespace Escargot {

// An assignment operator expression.
class AssignmentExpressionModNode : public ExpressionNode {
public:
    friend class ScriptParser;

    AssignmentExpressionModNode(Node* left, Node* right)
        : ExpressionNode()
    {
        m_left = left;
        m_right = right;
    }

    virtual ~AssignmentExpressionModNode()
    {
        delete m_left;
        delete m_right;
    }

    virtual ASTNodeType type() { return ASTNodeType::AssignmentExpressionMod; }
    virtual void generateExpressionByteCode(ByteCodeBlock* codeBlock, ByteCodeGenerateContext* context)
    {
        m_left->generateResolveAddressByteCode(codeBlock, context);
        m_left->generateReferenceResolvedAddressByteCode(codeBlock, context);
        size_t src0 = context->getLastRegisterIndex();
        m_right->generateExpressionByteCode(codeBlock, context);
        size_t src1 = context->getLastRegisterIndex();
        context->giveUpRegister();
        context->giveUpRegister();
        size_t dst = context->getRegister();
        codeBlock->pushCode(BinaryMod(ByteCodeLOC(m_loc.index), src0, src1, dst), context, this);
        m_left->generateStoreByteCode(codeBlock, context, false);
    }

protected:
    Node* m_left; // left: Pattern;
    Node* m_right; // right: Expression;
};
}

#endif
