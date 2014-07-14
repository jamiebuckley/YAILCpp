#ifndef LEXHEADER
#define LEXHEADER


#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define EOI   0
#define SEMI  1 
#define PLUS  2
#define MINUS 3
#define TIMES 4 
#define DIV   5
#define LP    6
#define RP    7
#define LCP   8
#define RCP   9
#define DO    10
#define WHI   11
#define IF    12
#define NUM   13
#define VAR   14
#define EQ    15

#define LT	  16
#define GT	  17


class Lexeme
{
	public:
		Lexeme(int type, std::string* value);
		~Lexeme();
		int type;
		std::string* value;
};

class Lexer
{
		public:
			Lexer();
			void LexFile(std::string filename);
			void PrintLexemes();
		private:
			std::vector<Lexeme*> lexemes;
			int Lex(std::string content);
			void AddLexeme(int type, std::string value);
};

#endif
