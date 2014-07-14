#include "Lex.h"

#include <sstream>
#include <cctype>

int main(int argc, char** argv)
{
	Lexer lexer;
	lexer.LexFile("test.yail");
	lexer.PrintLexemes();
}

/*********************
 * Lexeme Code
 *********************/

Lexeme::Lexeme(int type, std::string* value)
{
	this->type = type;
	this->value = value;
}

Lexeme::~Lexeme()
{
	delete value;
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
	for(std::vector<Lexeme*>::iterator i = lexemes.begin(); i != lexemes.end(); ++i)
	{
		Lexeme* l = *i;
		std::cout << *l->value << "\n";
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
			case '\0': AddLexeme(EOI, "\0"); current++; break;
			case ';':  AddLexeme(SEMI, ";"); current++; break; 
			case '+':  AddLexeme(PLUS, "+"); current++; break;
			case '-':  AddLexeme(MINUS, "-"); current++; break;
			case '*':  AddLexeme(TIMES, "*"); current++; break;
			case '/':  AddLexeme(DIV, "/"); current++; break;
			case '(':  AddLexeme(LP, "("); current++; break;
			case ')':  AddLexeme(RP, ")"); current++; break;
			case '{':  AddLexeme(LCP, "{"); current++; break;
			case '}':  AddLexeme(RCP, "}"); current++; break;
			case '=':  AddLexeme(EQ, "="); current++; break;
			case '<':  AddLexeme(LT, "<"); current++; break;
			case '>':  AddLexeme(GT, ">"); current++; break;
			case '\n':
			case '\t':
			case ' ': current++; break;	
			default:
				if(!content.compare(current * sizeof(char), 2 * sizeof(char), "if"))
				{
					AddLexeme(IF, "if");
					current+=2;
					break;
				}
				else if(!content.compare(current * sizeof(char), 2 * sizeof(char), "while"))
				{
					AddLexeme(IF, "while");
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
					AddLexeme(VAR, content.substr(current-llen, llen));
					break;
				}	
				if(isdigit(currentChar))
				{
					while(isdigit(currentChar))
					{
						current++; llen++;
						currentChar=content[current];
					}
					AddLexeme(NUM, content.substr(current-llen, llen));
				}
			}
	}
	return 0;
}

void Lexer::AddLexeme(int type, std::string value)
{
	std::string* v = new std::string(value);
	Lexeme* l = new Lexeme(type, v);
	lexemes.push_back(l);
}
