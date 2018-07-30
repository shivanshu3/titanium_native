#include <iostream>

#include "antlr4-runtime.h"
#include "calculatorLexer.h"
#include "calculatorParser.h"

#include <Windows.h>

#pragma execution_character_set("utf-8")

using namespace antlr4;

int main(int argc, const char * argv[]) {

	ANTLRInputStream input("1 + 2 = 3");
	calculatorLexer lexer(&input);
	CommonTokenStream tokens(&lexer);

	calculatorParser parser(&tokens);
	tree::ParseTree *tree = parser.equation();

	std::wstring s = antlrcpp::s2ws(tree->toStringTree(&parser)) + L"\n";

	// OutputDebugString(s.data()); // Only works properly since VS 2015.
	std::wcout << "Parse Tree: " << s << std::endl; // Unicode output in the console is very limited.

	system("PAUSE");

	return 0;;
}
