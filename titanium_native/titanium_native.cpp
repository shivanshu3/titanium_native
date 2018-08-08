// Antlr includes
#include "antlr4-runtime.h"
#include "titaniumNativeLexer.h"
#include "titaniumNativeParser.h"
#include "titaniumNativeVisitor.h"
#include "titaniumNativeBaseVisitor.h"

// System includes
#include <iostream>
#include <Windows.h>

// Project includes
#include "../util.h"

using namespace antlr4;

struct CustomVisitor : TitaniumNativeBaseVisitor
{
    virtual antlrcpp::Any visitTnExpression(TitaniumNativeParser::TnExpressionContext *ctx) override
    {
        auto tnTermCtxs = ctx->tnTerm();

        std::vector<std::string> terms;

        for (auto& ttermCtx : tnTermCtxs)
        {
            std::string term = visit(ttermCtx);
            terms.push_back(std::move(term));
        }

        return terms;
    }
    
    virtual antlrcpp::Any visitTnTerm(TitaniumNativeParser::TnTermContext *ctx) override
    {
        auto literalCtx = ctx->tnLiteral();

        TnAssert(literalCtx);

        return visit(literalCtx);
    }

    virtual antlrcpp::Any visitTnLiteral(TitaniumNativeParser::TnLiteralContext *ctx) override
    {
        auto foo = ctx->TN_NUMBER();
        auto bar = ctx->TN_BOOL();

        return ctx->getText();
    }
};

struct CustomErrorStrategy : DefaultErrorStrategy
{
    CustomErrorStrategy() : m_errorEncountered{false}
    {}

    bool errorEncountered()
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
    std::vector<std::string> terms = visitor.visitTnExpression(tree);

    std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

    std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

    ExitApplication(0);
}
