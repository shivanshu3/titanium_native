#pragma once

struct TitaniumTerm
{
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

private:
    bool m_value;
};

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
