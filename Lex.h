#ifndef LEXHEADER
#define LEXHEADER
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

const std::string LexemeTypeName[] = {"EOI", "SEMI", "PLUS", "MINUS", "TIMES", "DIV",
											"LP", "RP", "LCP", "RCP", "DO", "WHI", "IF", "NUM",
											"VAR", "EQ", "LT", "GT", "JUMP", "JUMPIF"};

enum class LexemeType
{
EOI, SEMI, PLUS, MINUS, TIMES, DIV,
LP, RP, LCP, RCP, DO, WHI, IF, NUM,
VAR, EQ, LT, GT, JUMP, JUMPIF
};

class Lexeme
{
	public:
		Lexeme(LexemeType type, std::string value);
		LexemeType type;
		std::string value;
};

class Lexer
{
		public:
			Lexer();
			void LexFile(std::string filename);
			void PrintLexemes();
			std::vector<Lexeme> lexemes;
		private:
			int Lex(std::string content);
			void AddLexeme(LexemeType type, std::string value);
};

#endif
