#include <iostream>

#include "antlr4-runtime.h"
#include "calculatorLexer.h"
#include "calculatorParser.h"
#include "calculatorVisitor.h"
#include "calculatorBaseVisitor.h"

#include <Windows.h>

using namespace antlr4;

struct CustomVisitor : calculatorBaseVisitor
{
    virtual antlrcpp::Any visitTexpression(calculatorParser::TexpressionContext *ctx) override
    {
        auto ttermCtxs = ctx->tterm();

        std::vector<std::string> terms;

        for (auto& ttermCtx : ttermCtxs)
        {
            std::string term = visit(ttermCtx);
            terms.push_back(std::move(term));
        }

        return terms;
    }
    
    virtual antlrcpp::Any visitTterm(calculatorParser::TtermContext *ctx) override
    {
        return ctx->getText();
    }
};

int main(int argc, const char * argv[])
{

    ANTLRInputStream input("30 11 123 554");
    calculatorLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    calculatorParser parser(&tokens);

    calculatorParser::TexpressionContext *tree = parser.texpression();

    CustomVisitor visitor;
    std::vector<std::string> terms = visitor.visitTexpression(tree);

    std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

    // OutputDebugString(s.data()); // Only works properly since VS 2015.
    std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

    system("PAUSE");

    return 0;
}

// EOF
