// Antlr includes
#include "antlr4-runtime.h"
#include "titaniumNativeLexer.h"
#include "titaniumNativeParser.h"
#include "titaniumNativeVisitor.h"
#include "titaniumNativeBaseVisitor.h"

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
    // Returns std::shared_ptr<TitaniumExpression>
    virtual antlrcpp::Any visitTnExpression(TitaniumNativeParser::TnExpressionContext *ctx) override
    {
        auto tnTermCtxs = ctx->tnTerm();

        std::vector<std::shared_ptr<TitaniumTerm>> terms;

        for (auto& ttermCtx : tnTermCtxs)
        {
            std::shared_ptr<TitaniumTerm> term = visit(ttermCtx);
            terms.push_back(std::move(term));
        }

        return std::make_shared<TitaniumExpression>(std::move(terms));
    }

    // Returns std::shared_ptr<TitaniumTerm>
    virtual antlrcpp::Any visitTnTerm(TitaniumNativeParser::TnTermContext *ctx) override
    {
        auto literalCtx = ctx->tnLiteral();

        TnAssert(literalCtx);

        std::shared_ptr<TitaniumLiteral> resultLiteral = visit(literalCtx);
        std::shared_ptr<TitaniumTerm> resultTerm = resultLiteral;

        return resultTerm;
    }

    // Returns std::shared_ptr<TitaniumLiteral>
    virtual antlrcpp::Any visitTnLiteral(TitaniumNativeParser::TnLiteralContext *ctx) override
    {
        std::shared_ptr<TitaniumLiteral> result;

        auto text = ctx->getText();

        if (ctx->TN_NUMBER())
        {
            double value = std::atof(text.data());
            result = std::make_shared<TitaniumNumberLiteral>(value);
        }
        else if (ctx->TN_BOOL())
        {
            bool value = (text == "true") ? true : false;
            result = std::make_shared<TitaniumBoolLiteral>(value);
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
    std::shared_ptr<TitaniumExpression> expressionAst = visitor.visitTnExpression(tree);

    std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

    std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

    ExitApplication(0);
}
