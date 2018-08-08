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
    virtual antlrcpp::Any visitTnOperator(TitaniumNativeParser::TnOperatorContext *ctx) override {
        return new TitaniumOperator{ ctx->getText() };
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

    CustomVisitor visitor;
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
