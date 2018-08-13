#include "stdafx.h"

// System includes
#include <iostream>

// Project includes
#include "../util.h"
#include "AstElements.h"
#include "parse.h"

int main(int argc, const char * argv[])
{
    std::unique_ptr<TitaniumExpression> titaniumExpression;
    
    try
    {
        titaniumExpression = ParseProgram("11.2 123 + true compare =foo !foo (2 3 +) `hello`");
    }
    catch (const std::string& err)
    {
        std::cerr << err << std::endl;
        ExitApplication(1);
    }

    std::wcout << L"The Titanium AST:" << std::endl;
    titaniumExpression->Print();

    ExitApplication(0);
}
