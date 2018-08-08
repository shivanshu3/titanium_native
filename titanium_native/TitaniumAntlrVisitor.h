#pragma once

// Note: The visitor functions return shared ptrs instead of unique ptrs because of this bug
// in Antlr:
// https://github.com/antlr/antlr4/issues/1855
struct TitaniumAntlrVisitor : TitaniumNativeBaseVisitor
{
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

        return new TitaniumExpression { std::move(terms) };
    }

    // Returns TitaniumTerm*
    virtual antlrcpp::Any visitTnTerm(TitaniumNativeParser::TnTermContext *ctx) override
    {
        if (ctx->tnLiteral())
        {
            TitaniumLiteral* resultLiteral = visit(ctx->tnLiteral());
            TitaniumTerm* resultTerm = resultLiteral;
            return resultTerm;
        }
        else if (ctx->tnOperator())
        {
            TitaniumOperator* resultOperator = visit(ctx->tnOperator());
            TitaniumTerm* resultTerm = resultOperator;
            return resultTerm;
        }
        else
        {
            TnAssert(false);
            return nullptr;
        }
    }

    // Returns TitaniumOperator*
    virtual antlrcpp::Any visitTnOperator(TitaniumNativeParser::TnOperatorContext *ctx) override
    {
        return new TitaniumOperator { ctx->getText() };
    }

    // Returns TitaniumLiteral*
    virtual antlrcpp::Any visitTnLiteral(TitaniumNativeParser::TnLiteralContext *ctx) override
    {
        TitaniumLiteral* result;

        auto text = ctx->getText();

        if (ctx->TN_NUMBER())
        {
            double value = std::atof(text.data());
            result = new TitaniumNumberLiteral { value };
        }
        else if (ctx->TN_BOOL())
        {
            bool value = (text == "true") ? true : false;
            result = new TitaniumBoolLiteral { value };
        }
        else
        {
            TnAssert(false);
        }

        return result;
    }
};
