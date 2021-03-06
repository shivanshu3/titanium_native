#pragma once

struct TitaniumAstNode
{
    virtual void Print() = 0;
};

enum class TitaniumTermType
{
    LITERAL,
    OPERATOR,
    PROCEDURE
};

struct TitaniumTerm : TitaniumAstNode
{
    virtual TitaniumTermType GetTermType() = 0;
};

struct TitaniumOperand : TitaniumTerm
{
};

struct TitaniumLiteral : TitaniumOperand
{
    virtual TitaniumTermType GetTermType() override
    {
        return TitaniumTermType::LITERAL;
    }
};

template<typename T>
struct ValueStore
{
    ValueStore(T value) : m_value { std::move(value) }
    {
    }

    const T& GetValue() const
    {
        return m_value;
    }

    void GenericPrint()
    {
        std::cout << m_value << std::endl;
    }

protected:
    T m_value;
};

template<typename T>
struct TitaniumGenericLiteral : ValueStore<T>, TitaniumLiteral
{
    TitaniumGenericLiteral(T t) : ValueStore<T> { std::move(t) }
    {}

    virtual void Print() override
    {
        ValueStore<T>::GenericPrint();
    }
};

using TitaniumNumberLiteral = TitaniumGenericLiteral<double>;

using TitaniumBoolLiteral = TitaniumGenericLiteral<bool>;

using TitaniumStringLiteral = TitaniumGenericLiteral<std::string>;

struct TitaniumOperator : TitaniumTerm
{
    virtual TitaniumTermType GetTermType() override
    {
        return TitaniumTermType::OPERATOR;
    }
};

struct TitaniumWordOperator : ValueStore<std::string>, TitaniumOperator
{
    TitaniumWordOperator(std::string identifier) : ValueStore<std::string> { std::move(identifier) }
    {}

    virtual void Print() override
    {
        ValueStore<std::string>::GenericPrint();
    }
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
    INCREMENT,
    DESCREMENT,
    ARRAY_ACCESS,
    LTE,
    GTE,
    DOUBLE_EQUAL,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    LT,
    GT,
};

struct TitaniumSymbolOperator : ValueStore<TitaniumSymbolType>, TitaniumOperator
{
    TitaniumSymbolOperator(TitaniumSymbolType symbol) : ValueStore<TitaniumSymbolType> { symbol }
    {}

    virtual void Print() override
    {
        std::cout << "symbol#" << static_cast<int>(m_value) << std::endl;
    }
};

// This is the top level AST node, which represents the entire Titanium program
struct TitaniumExpression : TitaniumAstNode
{
    TitaniumExpression(std::vector<std::unique_ptr<TitaniumTerm>> _terms) : m_terms{ std::move(_terms) }
    {}

    const std::vector<std::unique_ptr<TitaniumTerm>>& GetTerms() const
    {
        return m_terms;
    }

    virtual void Print() override
    {
        std::wcout << "Expression start" << std::endl;

        for (auto const& term : m_terms)
        {
            term->Print();
        }

        std::wcout << "Expression stop" << std::endl;
    }

private:
    std::vector<std::unique_ptr<TitaniumTerm>> m_terms;
};

struct TitaniumProcedure : TitaniumOperand
{
    TitaniumProcedure(std::unique_ptr<TitaniumExpression> expression) : m_expression { std::move(expression) }
    {
    }

    const TitaniumExpression& GetExpression() const
    {
        return *m_expression;
    }

    virtual void Print() override
    {
        std::wcout << "Procedure start" << std::endl;
        m_expression->Print();
        std::wcout << "Procedure end" << std::endl;
    }

    virtual TitaniumTermType GetTermType() override
    {
        return TitaniumTermType::PROCEDURE;
    }

private:
    std::unique_ptr<TitaniumExpression> m_expression;
};
