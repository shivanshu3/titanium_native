#include "stdafx.h"

// System includes
#include <iostream>
#include <Windows.h>
#include <cstdlib>

// Project includes
#include "../util.h"
#include "AstElements.h"

using namespace antlr4;

// Note: The visitor functions return shared ptrs instead of unique ptrs because of this bug
// in Antlr:
// https://github.com/antlr/antlr4/issues/1855
struct CustomVisitor : TitaniumNativeBaseVisitor
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

        return new TitaniumExpression{ std::move(terms) };
    }

    // Returns TitaniumTerm*
    virtual antlrcpp::Any visitTnTerm(TitaniumNativeParser::TnTermContext *ctx) override
    {
        auto literalCtx = ctx->tnLiteral();

        TnAssert(literalCtx);

        TitaniumLiteral* resultLiteral = visit(literalCtx);
        TitaniumTerm* resultTerm = resultLiteral;
        return resultTerm;
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
        else
        {
            TnAssert(false);
        }

        return result;
    }
};

struct CustomErrorStrategy : DefaultErrorStrategy
{
    CustomErrorStrategy() : m_errorEncountered{ false }
    {}

    bool errorEncountered() const
    {
        return m_errorEncountered;
    }

protected:
    virtual void beginErrorCondition(Parser *recognizer) override
    {
        std::cerr << "TN Error:" << std::endl;
        m_errorEncountered = true;
    }

private:
    bool m_errorEncountered;
};

int main(int argc, const char * argv[])
{
    ANTLRInputStream input("11.2 123 true 554.12");
    TitaniumNativeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TitaniumNativeParser parser(&tokens);

    auto errorStrategy = std::make_shared<CustomErrorStrategy>();
    parser.setErrorHandler(errorStrategy);

    TitaniumNativeParser::TnExpressionContext *tree = parser.tnExpression();

    if (errorStrategy->errorEncountered())
    {
        std::cerr << "Exiting due to parse failure." << std::endl;
        ExitApplication(1);
    }

    CustomVisitor visitor;
    TitaniumExpression* expressionAst = visitor.visitTnExpression(tree);

    std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

    std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

    ExitApplication(0);
}
