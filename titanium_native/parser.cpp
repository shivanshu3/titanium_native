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
#include "parser.h"

using namespace antlr4;

// Parses the given program and returns the AST.
// If the parse is not successful because of a lexer/parser error, then an
// exception is thrown of type std::string with the error message.
std::unique_ptr<TitaniumExpression> ParseProgram(const std::string& programInput)
{
    ANTLRInputStream input(programInput);
    TitaniumNativeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TitaniumNativeParser parser(&tokens);

    TitaniumAntlrErrorListener errorListener;
    lexer.addErrorListener(&errorListener);
    parser.addErrorListener(&errorListener);

    TitaniumNativeParser::TnProgramContext *tree = parser.tnProgram();

    if (errorListener.errorEncountered())
    {
        throw std::string{ "Exiting due to lexer/parser failure." };
    }

    TitaniumAntlrVisitor visitor;
    TitaniumExpression* expressionAst = visitor.visitTnProgram(tree);

    return std::unique_ptr<TitaniumExpression>{expressionAst};
}
