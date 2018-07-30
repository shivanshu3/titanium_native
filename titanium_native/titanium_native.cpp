#include <iostream>

#include "antlr4-runtime.h"
#include "calculatorLexer.h"
#include "calculatorParser.h"
#include "calculatorVisitor.h"
#include "calculatorBaseVisitor.h"

#include <Windows.h>

#pragma execution_character_set("utf-8")

using namespace antlr4;

struct CustomVisitor : calculatorBaseVisitor
{
	virtual antlrcpp::Any visitEquation(calculatorParser::EquationContext* ctx)
	{
		return ctx->getText();
	}
};

int main(int argc, const char * argv[]) {

	ANTLRInputStream input("dhfksjhk");
	calculatorLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	calculatorParser parser(&tokens);

	calculatorParser::EquationContext *tree = parser.equation();

	CustomVisitor visitor;
	std::string foo = visitor.visitEquation(tree);

	std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

	// OutputDebugString(s.data()); // Only works properly since VS 2015.
	std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

	system("PAUSE");

	return 0;;
}
