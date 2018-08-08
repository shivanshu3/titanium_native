#pragma once

struct TitaniumTerm
{
    virtual void Print() = 0;
};

struct TitaniumLiteral : TitaniumTerm
{
};

struct TitaniumNumberLiteral : TitaniumLiteral
{
    TitaniumNumberLiteral(double _number) : m_number{ _number }
    {}

    double GetNumber() const
    {
        return m_number;
    }

    virtual void Print() override
    {
        std::wcout << m_number << std::endl;
    }

private:
    double m_number;
};

struct TitaniumBoolLiteral : TitaniumLiteral
{
    TitaniumBoolLiteral(bool _value) : m_value{ _value }
    {}

    bool GetValue() const
    {
        return m_value;
    }

    virtual void Print() override
    {
        std::wcout << m_value << std::endl;
    }

private:
    bool m_value;
};

struct TitaniumOperator : TitaniumTerm
{
};

struct TitaniumWordOperator : TitaniumOperator
{
    TitaniumWordOperator(std::string identifier) : m_identifier{ std::move(identifier) }
    {}

    const std::string& GetName() const
    {
        return m_identifier;
    }

    virtual void Print() override
    {
        std::cout << m_identifier << std::endl;
    }

private:
    std::string m_identifier;
};

struct TitaniumVariableOperator : TitaniumOperator
{
    TitaniumVariableOperator(std::string variableName, bool assignment) :
        m_variableName{ std::move(variableName) },
        m_assignment{ assignment }
    {}

    const std::string& GetVariableName() const
    {
        return m_variableName;
    }

    bool IsAssignment() const
    {
        return m_assignment;
    }

    virtual void Print() override
    {
        char firstChar = m_assignment ? '=' : '!';
        std::cout << "var_op:{" << firstChar << m_variableName << '}' << std::endl;
    }

private:
    std::string m_variableName;
    bool m_assignment;
};

enum class TitaniumSymbolType
{
    INVALID,
    PLUS,
    MINUS,
    TIMES,
    DIV
};

struct TitaniumSymbolOperator : TitaniumOperator
{
    TitaniumSymbolOperator(TitaniumSymbolType symbol) : m_symbol{ symbol }
    {}

    TitaniumSymbolType GetSymbol() const
    {
        return m_symbol;
    }

    virtual void Print() override
    {
        std::string result;

        switch (m_symbol)
        {
            case TitaniumSymbolType::PLUS:
                result = "+";
                break;
            case TitaniumSymbolType::MINUS:
                result = "-";
                break;
            case TitaniumSymbolType::TIMES:
                result = "*";
                break;
            case TitaniumSymbolType::DIV:
                result = "/";
                break;
            default:
                result = "INVALID_SYMBOL";
                break;
        }

        std::cout << result << std::endl;
    }

private:
    TitaniumSymbolType m_symbol;
};

// This is the top level AST node, which represents the entire Titanium program
struct TitaniumExpression
{
    TitaniumExpression(std::vector<std::unique_ptr<TitaniumTerm>> _terms) : m_terms{ std::move(_terms) }
    {}

    const std::vector<std::unique_ptr<TitaniumTerm>>& GetTerms() const
    {
        return m_terms;
    }

private:
    std::vector<std::unique_ptr<TitaniumTerm>> m_terms;
};
