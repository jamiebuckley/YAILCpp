#ifndef PARSEHEADER
#define PARSEHEADER

#include <iostream>
#include <string>
#include "Lex.h"
#include <memory>

class ASTNode
{
	public:
		std::shared_ptr<ASTNode> left;
		std::shared_ptr<ASTNode> right;
		LexemeType type;
		std::string value;
		std::vector<std::shared_ptr<ASTNode>> subStatements;
		ASTNode(LexemeType type, std::string value);
};

class Parser
{
	public:
		void ParseFile(std::string filename);
		void Print();
		std::vector<std::shared_ptr<ASTNode>>& getStatements();

	private:
		Lexer lexer;
		unsigned int current;
		std::vector<std::shared_ptr<ASTNode>> statements;

		void PrintNode(const std::shared_ptr<ASTNode>& thisNode, int numTabs);
		void Statements();
		std::shared_ptr<ASTNode> Statement();
		std::shared_ptr<ASTNode> Parenthesized_Statement(bool isWhile);
		std::shared_ptr<ASTNode> Basic_Statement();
		std::shared_ptr<ASTNode> Var_Statement();
		std::shared_ptr<ASTNode> Expression();
		std::shared_ptr<ASTNode> ExpressionAlpha();
		std::shared_ptr<ASTNode> Term();
		std::shared_ptr<ASTNode> TermAlpha();
		std::shared_ptr<ASTNode> Factor();
		bool Match(LexemeType type);
		void Advance();
		Lexeme getLex();

};
#endif
