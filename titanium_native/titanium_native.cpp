#include "stdafx.h"

// System includes
#include <iostream>
#include <Windows.h>
#include <cstdlib>

// Project includes
#include "../util.h"
#include "AstElements.h"
#include "TitaniumAntlrVisitor.h"
#include "TitaniumAntlrErrorListener.h"

using namespace antlr4;

int main(int argc, const char * argv[])
{
    ANTLRInputStream input("11.2 123 + true compare =foo !foo");
    TitaniumNativeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TitaniumNativeParser parser(&tokens);

    TitaniumAntlrErrorListener errorListener;
    lexer.addErrorListener(&errorListener);
    parser.addErrorListener(&errorListener);

    TitaniumNativeParser::TnExpressionContext *tree = parser.tnExpression();

    if (errorListener.errorEncountered())
    {
        std::cerr << "Exiting due to lexer/parser failure." << std::endl;
        ExitApplication(1);
    }

    TitaniumAntlrVisitor visitor;
    TitaniumExpression* expressionAst = visitor.visitTnExpression(tree);

    // Printing the AST as parsed by us
    {
        std::wcout << L"Our custom parse tree:" << std::endl;
        
        for (const auto& term : expressionAst->GetTerms())
        {
            term->Print();
        }
    }

    // Printing the AST as parsed by ANTLR
    {
        std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";
        std::wcout << "ANTLR's parse tree:" << std::endl << s << std::endl;
    }

    ExitApplication(0);
}
