#include "ILC.h"

int main()
{
	Parser p;
	p.ParseFile("test.yail");	
	ILCGenerator ilcGenerator;
	ilcGenerator.GenerateILC(p);
	ilcGenerator.PrintILC();
}

VariableInfo::VariableInfo(std::string name)
{
	this->name = name;
}

void ILCLine::PrintLine()
{
	std::cout << lineNumber << ": " << 	
			LexemeTypeName[static_cast<int>(operation)] << " " << operand0 << " " << operand1 << " " << operand2 << "\n";
}

ILCGenerator::ILCGenerator()
{
	temporary = 0;
	line = 0;
}

void ILCGenerator::GenerateILC(Parser parser)
{
	auto statementList = parser.getStatements();
	for(auto current = statementList.begin(); current != statementList.end(); current++)
	{
		TranslateStatement(*current);	
	}
}

void ILCGenerator::PrintILC()
{
	std::cout<<"ILC:\n";
	for(auto currentLine = ilcList.begin(); currentLine != ilcList.end(); currentLine++)
	{
		(*currentLine)->PrintLine();	
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
	std::shared_ptr<ILCLine> thisLine(new ILCLine());
	if(VTable.find(astNode->value) == VTable.end())
	{
		std::shared_ptr<VariableInfo> v(new VariableInfo(astNode->value)); 
		std::pair<std::string, std::shared_ptr<VariableInfo>> mapPair(astNode->value, v);
		VTable.insert(mapPair);
	}	

	thisLine->operand0 = astNode->value;
	return thisLine;
}

std::shared_ptr<ILCLine> ILCGenerator::HandleASSIGN(const std::shared_ptr<ASTNode>& astNode)
{
	std::shared_ptr<ILCLine> thisLine(new ILCLine());
	thisLine->operation = astNode->type;

	std::string varName = astNode->left->value;
	if(astNode->right->type == LexemeType::NUM)
		thisLine->operand2 = astNode->right->value;
	else
		thisLine->operand2 = TranslateStatement(astNode->right)->operand0;

	thisLine->operand1 = astNode->left->value;

	thisLine->operand0 = thisLine->operand2;
	thisLine->lineNumber=nextLine();

	ilcList.push_back(thisLine);
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

	thisLine->lineNumber=nextLine();
	ilcList.push_back(thisLine);
	return thisLine;
}

std::shared_ptr<ILCLine> ILCGenerator::HandleLOOP(const std::shared_ptr<ASTNode>& astNode)
{
	std::shared_ptr<ILCLine> conditionLine = HandleOP(astNode->left);
	int conditionLineNumber = conditionLine->lineNumber; 
	std::shared_ptr<ILCLine> jumpEndLine(new ILCLine());
	jumpEndLine->operation = LexemeType::JUMPIF;	
	jumpEndLine->lineNumber=nextLine();
	ilcList.push_back(jumpEndLine);

	auto list = astNode->subStatements;
	for(auto current = list.begin(); current != list.end(); current++)
	{
		auto thisLine = TranslateStatement(*current);	
	}

	int endLineNumber = ilcList.back()->lineNumber;
	jumpEndLine->operand1 = std::to_string(endLineNumber);
	if(astNode->type == LexemeType::WHI)
	{
		std::shared_ptr<ILCLine> jumpBeginningLine(new ILCLine());
		jumpBeginningLine->operation = LexemeType::JUMP;
		jumpBeginningLine->operation = LexemeType::JUMPIF;	
		jumpBeginningLine->lineNumber=nextLine();
		jumpBeginningLine->operand1 = std::to_string(conditionLineNumber);
		ilcList.push_back(jumpBeginningLine);
	}
	return jumpEndLine;
}

std::string ILCGenerator::nextTemp()
{
	return "T" + std::to_string(temporary++);
}

int ILCGenerator::nextLine()
{
	return line++;
}
