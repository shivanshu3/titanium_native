#include <iostream>

#include "antlr4-runtime.h"
#include "titaniumNativeLexer.h"
#include "titaniumNativeParser.h"
#include "titaniumNativeVisitor.h"
#include "titaniumNativeBaseVisitor.h"

#include <Windows.h>

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
        return ctx->getText();
    }
};

int main(int argc, const char * argv[])
{

    ANTLRInputStream input("30 11 123 554");
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
