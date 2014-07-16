#include "Parser.h"

int main(int argc, char** argv)
{
	Parser parser;
	parser.ParseFile("test.yail");
}

ASTNode::ASTNode(LexemeType type, std::string value)
{
	this->type = type;
	this->value = value;
}

void Parser::ParseFile(std::string filename)
{
	lexer.LexFile(filename);
	lexer.PrintLexemes();
	current = 0;
	Statements();
	Print();
}

void Parser::Print()
{
	std::cout<<"Parser Output: \n";
	for(auto thisNode = statements.begin(); thisNode != statements.end(); thisNode++)
	{
		std::shared_ptr<ASTNode>& i = *thisNode;
		PrintNode(i, 0);	
	}
}

inline void printTabs(int numTabs)
{
	for(int i = 0; i < numTabs; i++)
		std::cout << " ";
}

void Parser::PrintNode(const std::shared_ptr<ASTNode>& thisNode, int numTabs)
{
		if(thisNode->type == LexemeType::VAR || thisNode->type == LexemeType::NUM)
		{
			printTabs(numTabs);
			std::cout << "<'" << thisNode->value << "'/>\n";
		}
		else
		{
			printTabs(numTabs);
			std::cout << "<'" << thisNode->value << "'>\n";
			if(thisNode->type == LexemeType::WHI || thisNode->type == LexemeType::IF)
			{
				if(thisNode->left){ PrintNode(thisNode->left, numTabs+1); }
				for(auto statNode = thisNode->subStatements.begin(); statNode != thisNode->subStatements.end(); statNode++)
				{
					std::shared_ptr<ASTNode>& i = *statNode;
					PrintNode(i, numTabs+1);
				}	
			}
			else
			{
				if(thisNode->left){ PrintNode(thisNode->left, numTabs+1); }
				if(thisNode->right){ PrintNode(thisNode->right, numTabs+1); }
			}
			printTabs(numTabs);
			std::cout << "<'/" << thisNode->value << "'>\n";
		}
}

void Parser::Statements()
{
	while(current < lexer.lexemes.size()-1)
	{
		statements.push_back(Statement());	
	}
}

std::shared_ptr<ASTNode> Parser::Statement()
{
	if(Match(LexemeType::WHI))
		return Parenthesized_Statement(true);
	else if(Match(LexemeType::IF))
		return Parenthesized_Statement(false);
	else if(Match(LexemeType::VAR))
	{
		std::shared_ptr<ASTNode> bStat = Basic_Statement();
		if(Match(LexemeType::SEMI))
		{
			Advance();	
		}				
		else
		{
			std::cout<<"Expected Semicolon\n";
		}
		return bStat;
	}
	else
	{
		std::cerr<< "Unexpected Statement Beginning\n";
		std::exit(1);
	}
}

std::shared_ptr<ASTNode> Parser::Parenthesized_Statement(bool isWhile)
{
	LexemeType loopType = (isWhile)? LexemeType::WHI : LexemeType::IF;
	std::shared_ptr<ASTNode> ParenthesizedNode(new ASTNode(loopType, getLex().value));

	Advance();
	if(!Match(LexemeType::LP))
	{
		std::cerr<<"Expected open parenthesis after condition\n";
		std::exit(1);
	}
	Advance();
	
	ParenthesizedNode->left = Expression();

	if(!Match(LexemeType::RP))
	{
		std::cerr<<"Expected close parenthesis after condition\n";
		std::exit(1);
	}
	Advance();
	if(!Match(LexemeType::LCP))
	{
		std::cerr<<"Expected open '{' before statement list\n";
		std::exit(1);
	}
	Advance();
	while(!Match(LexemeType::RCP))
	{
		std::shared_ptr<ASTNode> thisPtr = Statement();
		ParenthesizedNode->subStatements.push_back(std::move(thisPtr));
		if(Match(LexemeType::EOI))
		{
			std::cerr<<"Unexpected EOF\n";
			std::exit(1);
		}
	}
	Advance();
	
	return ParenthesizedNode;
}

std::shared_ptr<ASTNode> Parser::Basic_Statement()
{
	std::shared_ptr<ASTNode> basicNode(new ASTNode(LexemeType::EQ, "="));
	basicNode->left = Var_Statement();
	basicNode->right = Expression();
	return basicNode;
}

std::shared_ptr<ASTNode> Parser::Var_Statement()
{
	std::shared_ptr<ASTNode> varNode(new ASTNode(LexemeType::VAR, getLex().value));
	varNode->value = getLex().value;
	Advance();

	if(Match(LexemeType::EQ))
		Advance();
	else
	{
		std::cerr << "Not a valid variable name\n";
	}

	return varNode;
}

std::shared_ptr<ASTNode> Parser::Expression()
{
	std::shared_ptr<ASTNode> termNode = Term();
	std::shared_ptr<ASTNode> expAl = ExpressionAlpha();

	if(expAl == NULL)
		return termNode;
	
	expAl->left = termNode;
	return expAl;
}

std::shared_ptr<ASTNode> Parser::ExpressionAlpha()
{
	Lexeme l = getLex();
	switch(l.type)
	{
		case LexemeType::GT:
		case LexemeType::LT:
		case LexemeType::PLUS:
		case LexemeType::MINUS:
		{
			std::shared_ptr<ASTNode> expAlNode(new ASTNode(l.type, l.value));
			Advance();
			std::shared_ptr<ASTNode> termNode = Term();

			std::shared_ptr<ASTNode> nextExpAlNode;
			if((nextExpAlNode = ExpressionAlpha()) == NULL)
			{
				expAlNode->right = termNode;
			}
			else
			{
				nextExpAlNode->left = termNode;
				expAlNode->right = nextExpAlNode;
			}
			return expAlNode;
		}
		default:
			return NULL;
	}
}

std::shared_ptr<ASTNode> Parser::Term()
{
	std::shared_ptr<ASTNode> factor = Factor();
	std::shared_ptr<ASTNode> term = TermAlpha();

	if(term == NULL)
		return factor;

	term->left = factor;
	return term;
}

std::shared_ptr<ASTNode> Parser::TermAlpha()
{
	Lexeme l = getLex();
	switch(l.type)
	{
		case LexemeType::TIMES:
		case LexemeType::DIV:
		{
			std::shared_ptr<ASTNode> termAlNode(new ASTNode(l.type, l.value));
			Advance();
			std::shared_ptr<ASTNode> factorNode = Factor();

			std::shared_ptr<ASTNode> nextTermAlNode;
			if((nextTermAlNode = TermAlpha()) == NULL)
			{
				termAlNode->right = factorNode;
			}
			else
			{
				nextTermAlNode->left = factorNode;
				termAlNode->right = nextTermAlNode;
			}
			return termAlNode;
		}
		default:
			return NULL;
	}
}

std::shared_ptr<ASTNode> Parser::Factor()
{
	Lexeme l = getLex();
	std::shared_ptr<ASTNode> factorNode(new ASTNode(l.type, l.value));
	factorNode->value = l.value;		
	Advance();
	return factorNode;
}

bool Parser::Match(LexemeType type)
{
	return (getLex().type == type);	
}

void Parser::Advance()
{
	std::cout << (getLex().value) <<"\n";
	current++;
}

Lexeme Parser::getLex()
{
	return lexer.lexemes[current];
}
