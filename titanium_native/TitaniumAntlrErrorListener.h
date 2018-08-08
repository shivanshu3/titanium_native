#pragma once

using namespace antlr4;

struct TitaniumAntlrErrorListener : BaseErrorListener
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
