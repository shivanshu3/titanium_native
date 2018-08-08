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
    TitaniumOperator(std::string text) : m_text{ std::move(text) }
    {}

    const std::string& GetText() const
    {
        return m_text;
    }

    virtual void Print() override
    {
        std::cout << m_text << std::endl;
    }

private:
    std::string m_text;
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
