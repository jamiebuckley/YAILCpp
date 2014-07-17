#include "ILC.h"

int main()
{
	Parser p;
	p.ParseFile("test.yail");	
	ILCGenerator ilcGenerator;
	ilcGenerator.GenerateILC(p);
}

VariableInfo::VariableInfo(std::string name)
{
	this->name = name;
}

ILCGenerator::ILCGenerator()
{
	temporary = 0;
}

void ILCGenerator::GenerateILC(Parser parser)
{
	auto statementList = parser.getStatements();
	for(auto current = statementList.begin(); current != statementList.end(); current++)
	{
		TranslateStatement(*current);	
	}
}

std::shared_ptr<ILCLine> ILCGenerator::TranslateStatement(const std::shared_ptr<ASTNode>& astNode)
{
	switch(astNode->type)
	{
		case LexemeType::EQ:
				return HandleASSIGN(astNode);
		case LexemeType::VAR:
				return HandleVAR(astNode);
		case LexemeType::PLUS:
		case LexemeType::MINUS:
		case LexemeType::TIMES:
		case LexemeType::DIV:
		case LexemeType::GT:
		case LexemeType::LT:
				return HandleOP(astNode);
		case LexemeType::WHI:
		case LexemeType::IF:
				return HandleLOOP(astNode);
		default:
				return nullptr;
	}
}

std::shared_ptr<ILCLine> ILCGenerator::HandleVAR(const std::shared_ptr<ASTNode>& astNode)
{
	if(VTable.find(astNode->value) == VTable.end())
	{
		std::shared_ptr<VariableInfo> v(new VariableInfo(astNode->value)); 
		std::pair<std::string, std::shared_ptr<VariableInfo>> mapPair(astNode->value, v);
		VTable.insert(mapPair);
	}	
	return NULL;
}

std::shared_ptr<ILCLine> ILCGenerator::HandleASSIGN(const std::shared_ptr<ASTNode>& astNode)
{
	std::shared_ptr<ILCLine> thisLine(new ILCLine());
	thisLine->operation = astNode->type;

	std::string varName = astNode->left->value;
	if(astNode->right->type == LexemeType::NUM)
		thisLine->operand2 = astNode->right->value;
	else
		thisLine->operand1 = TranslateStatement(astNode->right)->operand0;

	thisLine->operand0 = operand2;

	return thisLine;
}

std::shared_ptr<ILCLine> ILCGenerator::HandleOP(const std::shared_ptr<ASTNode>& astNode)
{
	std::shared_ptr<ILCLine> thisLine(new ILCLine());
	thisLine->operation = astNode->type;

	/* Operands are either a constant value, or the destination of the calulating line */
	thisLine->operand1 = (astNode->left->type == LexemeType::NUM)? astNode->left->value :
		   TranslateStatement(astNode->left)->operand0;
	thisLine->operand2 = (astNode->right->type == LexemeType::NUM)? astNode->right->value :
		   TranslateStatement(astNode->right)->operand0;

	thisLine->operand0 = nextTemp();

	return thisLine;
}

std::shared_ptr<ILCLine> ILCGenerator::HandleLOOP(const std::shared_ptr<ASTNode>& astNode)
{
	std::shared_ptr<ILCLine> thisLine(new ILCLine());
	thisLine->operation = astNode->type;
	
	/* finish this */

	return thisLine;
}

int ILCGenerator::nextTemp()
{
	return temporary++;
}
