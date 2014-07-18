#include "Lex.h"

#include <sstream>
#include <cctype>

/*
int main(int argc, char** argv)
{
	Lexer lexer;
	lexer.LexFile("test.yail");
	lexer.PrintLexemes();
}
*/

/*********************
 * Lexeme Code
 *********************/

Lexeme::Lexeme(LexemeType type, std::string value)
{
	this->type = type;
	this->value = value;
}

/*********************
 * LEXER CODE 
 *********************/

Lexer::Lexer()
{
	/* Nothing to do here at the moment */
}

void Lexer::LexFile(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	
	if(file.is_open())
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		Lex(buffer.str());
	}
	else
		std::cout << "Error opening file" << filename << "\n";

	file.close();
}

void Lexer::PrintLexemes()
{
	std::cout << "Lexer Output:\n";
	int k = 0;
	for(auto i = lexemes.begin(); i != lexemes.end()-2; i++, ++k)
	{
		Lexeme l = *i;
		std::cout << k << ": " << l.value << "\n";
	}
	std::cout << "\n";
}

int Lexer::Lex(std::string content)
{
	unsigned int current = 0;
	while(current < content.length())
	{
		char currentChar = content[current];
		
		while(isspace(currentChar))
		{
			current++;
			currentChar=content[current];
		}

		switch(currentChar)
		{
				case '\0': AddLexeme(LexemeType::EOI, "\0"); current++; break;
				case ';':  AddLexeme(LexemeType::SEMI, ";"); current++; break; 
				case '+':  AddLexeme(LexemeType::PLUS, "+"); current++; break;
				case '-':  AddLexeme(LexemeType::MINUS, "-"); current++; break;
				case '*':  AddLexeme(LexemeType::TIMES, "*"); current++; break;
				case '/':  AddLexeme(LexemeType::DIV, "/"); current++; break;
				case '(':  AddLexeme(LexemeType::LP, "("); current++; break;
				case ')':  AddLexeme(LexemeType::RP, ")"); current++; break;
				case '{':  AddLexeme(LexemeType::LCP, "{"); current++; break;
				case '}':  AddLexeme(LexemeType::RCP, "}"); current++; break;
				case '=':  AddLexeme(LexemeType::EQ, "="); current++; break;
				case '<':  AddLexeme(LexemeType::LT, "<"); current++; break;
				case '>':  AddLexeme(LexemeType::GT, ">"); current++; break;
			case '\n':
			case '\t':
			case ' ': current++; break;	
			default:
				if(!content.compare(current * sizeof(char), 2 * sizeof(char), "if"))
				{
					AddLexeme(LexemeType::IF, "if");
					current+=2;
					break;
				}
				else if(!content.compare(current * sizeof(char), 2 * sizeof(char), "while"))
				{
					AddLexeme(LexemeType::WHI, "while");
					current += 5;
					break;
				}
				int llen = 0;
				if(isalpha(currentChar) && !ispunct(currentChar))
				{
					current++; llen++;
					while(isalnum(current) && !ispunct(current))
					{
						current++; llen++;
						currentChar=content[current];
					}
					AddLexeme(LexemeType::VAR, content.substr(current-llen, llen));
					break;
				}	
				if(isdigit(currentChar))
				{
					while(isdigit(currentChar))
					{
						current++; llen++;
						currentChar=content[current];
					}
					AddLexeme(LexemeType::NUM, content.substr(current-llen, llen));
				}
			}
	}
	return 0;
}

void Lexer::AddLexeme(LexemeType type, std::string value)
{
	std::string v = std::string(value);
	Lexeme l = Lexeme(type, v);
	lexemes.push_back(l);
}
