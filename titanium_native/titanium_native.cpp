#include "stdafx.h"

// System includes
#include <iostream>
#include <Windows.h>
#include <cstdlib>

// Project includes
#include "../util.h"
#include "AstElements.h"
#include "TitaniumAntlrVisitor.h"

using namespace antlr4;

struct CustomErrorListener : BaseErrorListener
{
    bool errorEncountered() const
    {
        return m_errorEncountered;
    }

protected:
    virtual void syntaxError(Recognizer *recognizer, Token * offendingSymbol, size_t line, size_t charPositionInLine,
        const std::string &msg, std::exception_ptr e) override
    {
        m_errorEncountered = true;
    }

    virtual void reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact,
        const antlrcpp::BitSet &ambigAlts, atn::ATNConfigSet *configs) override
    {
        m_errorEncountered = true;
    }

    virtual void reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
        const antlrcpp::BitSet &conflictingAlts, atn::ATNConfigSet *configs) override
    {
        m_errorEncountered = true;
    }

    virtual void reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
        size_t prediction, atn::ATNConfigSet *configs) override
    {
        m_errorEncountered = true;
    }

private:
    bool m_errorEncountered = false;
};

int main(int argc, const char * argv[])
{
    ANTLRInputStream input("11.2 123 + true compare =foo !foo");
    TitaniumNativeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    TitaniumNativeParser parser(&tokens);

    CustomErrorListener errorListener;
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
