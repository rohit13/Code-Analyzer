/////////////////////////////////////////////////////////////////////
//  MetricsAnalysis.h - Cpp file for MetricAnalysis.h			   //
//  ver 1.0                                                        //
//  Language:      C++, Visual Studio 2015						   //
//	Application:   Project#2 - CSE 687-Object Oriented Design	   //
//  Platform:      Dell XPS L510X, Windows 10					   //
//  Author:        Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class to get the analysis on the AST
that was built by parsing the source file.
It provides methods to print the analysis done on the each
node of the tree and the beutification around it.

Public Interface:
=================
MetricsAnalyzer(SyntaxTree* ast) //constructor
void getAnalysis(ASTNode* currNode)	//prints the analysis
void printHeader() //print header before the analysis info
void printFooter() //print footer after the analysis info

Build Process:
==============
Required files: MetricsAnalysis.h, Utilities.h
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release
*/

#include "MetricsAnalysis.h"
#include <iomanip>
#include "../Utilities/Utilities.h"
using stringHelp = Utilities::StringHelper;


//----< method to get metric analysis report and print it on console>--------------------------------
void MetricsAnalyzer::getAnalysis(ASTNode* currNode)
{
	if (currNode->getType() == "function") {
		std::cout << "* " << std::left << std::setw(nameRange) << std::setfill(separator) << currNode->getName()
			<< std::left << std::setw(numRange) << std::setfill(separator) << currNode->getLineCount()
			<< std::left << std::setw(numRange) << std::setfill(separator) << currNode->getComplexity()
			<< " *" << std::endl;
	}
	std::vector<ASTNode*>::iterator it;
	std::vector<ASTNode*> *children = currNode->getChild();
	for (it = children->begin(); it != children->end(); it++)
	{
		getAnalysis(*it);
	}
}

//----< method to print header of analysis report on console>--------------------------------
void MetricsAnalyzer::printHeader()
{
	stringHelp::Title("Functions summary");
	std::cout << std::endl;
	std::cout << std::left << std::setw(totalRange) << std::setfill('*') << "" << std::endl;
	std::cout << "* " << std::left << std::setw(nameRange) << std::setfill(separator) << "Function Name"
		<< std::left << std::setw(numRange) << std::setfill(separator) << "Lines"
		<< std::left << std::setw(numRange) << std::setfill(separator) << "Complexity"
		<< " *" << std::endl;
	std::cout << std::left << std::setw(totalRange) << std::setfill('*') << "" << std::endl;
}

//----< method to print footer of analysis report on console>--------------------------------
void MetricsAnalyzer::printFooter()
{
	std::cout << std::left << std::setw(totalRange) << std::setfill('*') << "" << std::endl;
}


//test stub
#ifdef TEST_METRICSANALYSIS

int main()
{
	std::cout << "\n  Testing Metrics Analysis";
	std::cout << "\n =====================\n";

	std::cout << "\n  pushing items onto AST";
	std::cout << "\n --------------------------------------------------------------------";

	AbstractSyntaxTree* testAST;
	element* pItem = new element;

	pItem->type = "function";
	pItem->name = "fun1";
	pItem->lineCount = 33;
	testAST = new AbstractSyntaxTree(pItem);

	pItem = new element;
	pItem->type = "if";
	pItem->name = "";
	pItem->lineCount = 38;
	testAST->getRoot()->addChild(pItem);

	pItem = new element;
	pItem->type = "for";
	pItem->name = "";
	pItem->lineCount = 50;
	testAST->getRoot()->addChild(pItem);

	testAST->getComplexity(testAST->getRoot());
	testAST->TreeWalk(testAST->getRoot());
	std::cout << std::endl;

	std::cout << "\n  printing the analysis";
	std::cout << "\n --------------------------------------------------------------------";
	MetricsAnalyzer* ma;
	ma = new MetricsAnalyzer(testAST);
	ma->getAnalysis(testAST->getRoot());
	std::cout << std::endl;
	return 0;
}

#endif