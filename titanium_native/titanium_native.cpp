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
        // remove:
        auto text = ctx->getText();

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

int main(int argc, const char * argv[])
{
    ANTLRInputStream input("11.2 123 true blah 554.12");
    TitaniumNativeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TitaniumNativeParser parser(&tokens);

    TitaniumNativeParser::TnExpressionContext *tree = parser.tnExpression();

    CustomVisitor visitor;
    std::vector<std::string> terms = visitor.visitTnExpression(tree);

    std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

    std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

    system("PAUSE");

    return 0;
}
