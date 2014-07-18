#ifndef ILCHEADER
#define ILCHEADER

#include <map>
#include <iostream>
#include <string>
#include "Parser.h"
#include "Lex.h"

class VariableInfo
{
	public:
		VariableInfo(std::string name);
		std::string name;
};

class ILCLine
{
	public:
	void PrintLine();	
	int lineNumber;	
	LexemeType operation;
	std::string operand0;
	std::string operand1;
	std::string operand2;
};

class ILCGenerator
{
	public:
		ILCGenerator();
 		void GenerateILC(Parser parser);
		void PrintILC();
		std::shared_ptr<ILCLine> TranslateStatement(const std::shared_ptr<ASTNode>& astNode);
		std::shared_ptr<ILCLine> HandleVAR(const std::shared_ptr<ASTNode>& astNode);
		std::shared_ptr<ILCLine> HandleASSIGN(const std::shared_ptr<ASTNode>& astNode);
		std::shared_ptr<ILCLine> HandleOP(const std::shared_ptr<ASTNode>& astNode);
		std::shared_ptr<ILCLine> HandleLOOP(const std::shared_ptr<ASTNode>& astNode);

	private:
		std::string nextTemp();
		int nextLine();
		int temporary;
		int line;

		std::map<std::string, std::shared_ptr<VariableInfo>> VTable;
		std::vector<std::shared_ptr<ILCLine>> ilcList;
};

#endif
