#include "Parser.h"

int main(int argc, char** argv)
{
	Lexer lexer;
	lexer.LexFile("test.yail");
	lexer.PrintLexemes();
}
