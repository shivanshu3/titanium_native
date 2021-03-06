#pragma once

using namespace antlr4;

// Note: The visitor functions return shared ptrs instead of unique ptrs because of this bug
// in Antlr:
// https://github.com/antlr/antlr4/issues/1855
struct TitaniumAntlrVisitor : TitaniumNativeBaseVisitor
{
    // Returns TitaniumExpression*
    virtual antlrcpp::Any visitTnProgram(TitaniumNativeParser::TnProgramContext *ctx) override
    {
        return visit(ctx->tnExpression());
    }

    // Returns TitaniumExpression*
    virtual antlrcpp::Any visitTnExpression(TitaniumNativeParser::TnExpressionContext *ctx) override
    {
        auto tnTermCtxs = ctx->tnTerm();

        std::vector<std::unique_ptr<TitaniumTerm>> terms;

        for (auto& ttermCtx : tnTermCtxs)
        {
            TitaniumTerm* term = visit(ttermCtx);
            terms.push_back(std::unique_ptr<TitaniumTerm>{ term });
        }

        return new TitaniumExpression{ std::move(terms) };
    }

    // Returns TitaniumTerm*
    virtual antlrcpp::Any visitTnTerm(TitaniumNativeParser::TnTermContext *ctx) override
    {
        TitaniumTerm* resultTerm = nullptr;

        if (ctx->tnOperand())
        {
            TitaniumOperand* resultOperand = visit(ctx->tnOperand());
            resultTerm = resultOperand;
        }
        else if (ctx->tnOperator())
        {
            TitaniumOperator* resultOperator = visit(ctx->tnOperator());
            resultTerm = resultOperator;
        }
        else
        {
            TnAssert(false);
        }

        return resultTerm;
    }

    // Returns TitaniumOperand*
    virtual antlrcpp::Any visitTnOperand(TitaniumNativeParser::TnOperandContext *ctx) override
    {
        TitaniumOperand* resultOperand = nullptr;

        if (ctx->tnLiteral())
        {
            TitaniumLiteral* resultLiteral = visit(ctx->tnLiteral());
            resultOperand = resultLiteral;
        }
        else if (ctx->tnProcedure())
        {
            TitaniumProcedure* resultProcedure = visit(ctx->tnProcedure());
            resultOperand = resultProcedure;
        }
        else
        {
            TnAssert(false);
        }

        return resultOperand;
    }

    // Returns TitaniumProcedure*
    virtual antlrcpp::Any visitTnProcedure(TitaniumNativeParser::TnProcedureContext *ctx) override
    {
        TitaniumExpression* expression = visit(ctx->tnExpression());
        std::unique_ptr<TitaniumExpression> expressionPtr { expression };

        return new TitaniumProcedure { std::move(expressionPtr) };
    }

    // Returns TitaniumOperator*
    virtual antlrcpp::Any visitTnOperator(TitaniumNativeParser::TnOperatorContext *ctx) override
    {
        TitaniumOperator* _operator = nullptr;

        if (ctx->tnWordOperator())
        {
            TitaniumWordOperator* wordOperator = visit(ctx->tnWordOperator());
            _operator = wordOperator;
        }
        else if (ctx->tnVariableOperator())
        {
            TitaniumVariableOperator* variableOperator = visit(ctx->tnVariableOperator());
            _operator = variableOperator;
        }
        else if (ctx->tnSymbolOperator())
        {
            TitaniumSymbolOperator* symbolOperator = visit(ctx->tnSymbolOperator());
            _operator = symbolOperator;
        }
        else
        {
            TnAssert(false);
        }

        return _operator;
    }

    // Returns TitaniumWordOperator*
    virtual antlrcpp::Any visitTnWordOperator(TitaniumNativeParser::TnWordOperatorContext *ctx) override
    {
        return new TitaniumWordOperator{ ctx->getText() };
    }

    // Returns TitaniumVariableOperator*
    virtual antlrcpp::Any visitTnVariableOperator(TitaniumNativeParser::TnVariableOperatorContext *ctx) override
    {
        auto text = ctx->getText();

        bool assignment = text[0] == '=';

        if (!assignment)
        {
            TnAssert(text[0] == '!');
        }

        // "text" will store the identifier name after this statement
        text.erase(text.begin());

        return new TitaniumVariableOperator{ std::move(text), assignment };
    }

    // Returns TitaniumSymbolOperator*
    virtual antlrcpp::Any visitTnSymbolOperator(TitaniumNativeParser::TnSymbolOperatorContext *ctx) override
    {
        auto text = ctx->getText();

        TitaniumSymbolType symbol;

        if (text == "++")
        {
            symbol = TitaniumSymbolType::INCREMENT;
        }
        else if (text == "--")
        {
            symbol = TitaniumSymbolType::DESCREMENT;
        }
        else if (text == "[]")
        {
            symbol = TitaniumSymbolType::ARRAY_ACCESS;
        }
        else if (text == "<=")
        {
            symbol = TitaniumSymbolType::LTE;
        }
        else if (text == ">=")
        {
            symbol = TitaniumSymbolType::GTE;
        }
        else if (text == "==")
        {
            symbol = TitaniumSymbolType::DOUBLE_EQUAL;
        }
        else if (text == "+")
        {
            symbol = TitaniumSymbolType::PLUS;
        }
        else if (text == "-")
        {
            symbol = TitaniumSymbolType::MINUS;
        }
        else if (text == "*")
        {
            symbol = TitaniumSymbolType::TIMES;
        }
        else if (text == "/")
        {
            symbol = TitaniumSymbolType::DIV;
        }
        else if (text == "<")
        {
            symbol = TitaniumSymbolType::LT;
        }
        else if (text == ">")
        {
            symbol = TitaniumSymbolType::GT;
        }
        else
        {
            TnAssert(false);
        }

        return new TitaniumSymbolOperator{ symbol };
    }

    // Returns TitaniumLiteral*
    virtual antlrcpp::Any visitTnLiteral(TitaniumNativeParser::TnLiteralContext *ctx) override
    {
        TitaniumLiteral* result;

        auto text = ctx->getText();

        if (ctx->TN_NUMBER())
        {
            double value = std::atof(text.data());
            result = new TitaniumNumberLiteral{ value };
        }
        else if (ctx->TN_BOOL())
        {
            bool value = (text == "true") ? true : false;
            result = new TitaniumBoolLiteral{ value };
        }
        else if (ctx->tnString())
        {
            TitaniumStringLiteral* stringLiteral = visit(ctx->tnString());
            result = stringLiteral;
        }
        else
        {
            TnAssert(false);
        }

        return result;
    }

    // Returns TitaniumStringLiteral*
    virtual antlrcpp::Any visitTnString(TitaniumNativeParser::TnStringContext *ctx) override
    {
        auto text = ctx->getText();

        size_t numCharsToDeleteFromFront = -1;

        if (ctx->TN_NON_VERBATIM_STRING())
        {
            numCharsToDeleteFromFront = 1;

            // TODO: Handle escape sequence
        }
        else if (ctx->TN_VERBATIM_STRING())
        {
            numCharsToDeleteFromFront = 2;
        }
        else
        {
            TnAssert(false);
        }

        text.erase(0, numCharsToDeleteFromFront);
        text.erase(text.end() - 1);

        return new TitaniumStringLiteral { std::move(text) };
    }
};
