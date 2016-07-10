/////////////////////////////////////////////////////////////////////
//  MetricExecutive.h - This class defines getFiles and			   //
//						executeParsing methods.					   //
//						Starting point for application.			   //
//  ver 1.0                                                        //
//  Lanaguage:     C++ Visual Studio 2015                          //
//  Platform:      Dell XPS L510X - Windows 10		               //
//	Application:   Project#2 - CSE 687-Object Oriented Design	   //
//  Author:        Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This class defines getFiles and executeParsing methods.Starting point for application.

Public Interface:
=================
int getFiles(int argc, char* argv[]); // method to get files
void demonstrateReqs(); // method to demonstrate all requirements
void demonstrateReq68Output(SyntaxTree* treeRoot); //method to demo output of requirements 6 and 8
void executeParsing(); // method to execute parsing on each file found at given directory path
int main(int argc, char* argv[]) //main method to start application with.

Build Process:
==============
Required files
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "../FileMgr/FileMgr.h"
#include "MetricExecutive.h"
#include "../DataStore/DataStore.h"
#include "../FileMgr/FileSystem.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include "../Parser/ActionsAndRules.h"
#include<conio.h>

#define Util StringHelper
using namespace Utilities;

//----< method to demonstrate all requirements>--------------------------------
void MetricExecutive::demonstrateReqs() {
	StringHelper::title("Demonstrating - Requirement 3");
	std::cout << std::endl;
	std::cout << std::endl << " Provided C++ packages for analyzing function size and complexity metrics for a set of specified packages. Please check MetricExecutive, MetricAnalyzer,";
	std::cout << std::endl << " Parser,SemiExp, and Tokenizer packages." << std::endl;
	StringHelper::title("Demonstrating - Requirement 4");
	std::cout << std::endl;
	std::cout << std::endl << " Provided a Parser package with a Parser class that is a container for Rules and that provides the interfaces IRule and IAction for rules contained in the";
	std::cout << std::endl << " Parser and actions contained in each rule. Please check Parser.h and Parser.cpp files." << std::endl;
	StringHelper::title("Demonstrating - Requirement 5");
	std::cout << std::endl;
	std::cout << std::endl << " Provided rules in ActionAndRules.h file to detect functions, containers, control statements, anonymous scopes and lambdas. Provided corresponding actions";
	std::cout << std::endl << " that supports an Abstract Syntax Tree which identifies construct type, lines and complexity." << std::endl;
	StringHelper::title("Demonstrating - Requirement 6");
	std::cout << std::endl;
	std::cout << " Provided a facility for building an abstract syntax tree that provides an interface for adding scope nodes to the tree.";
	std::cout << std::endl << " and methods to analyze the contents of the tree. Please check AST package.";
	std::cout << std::endl;
	StringHelper::title("Demonstrating - Requirement 7");
	std::cout << std::endl;
	std::cout << std::endl << " Provided FileMgr package that supports finding files and/or directories in a directory tree rooted at a specified path."
		" Please check FileMgr package." << std::endl;

	StringHelper::title("Demonstrating - Requirement 8");
	std::cout << std::endl;
	std::cout << std::endl << " Provided MetricAnalysis package which runs parser on provided files and construct AST with function summary.";
	std::cout << std::endl << " Please check MetricAnalysis.h and MetricAnalysis.cpp in MetricAnalysis package." << std::endl;

	StringHelper::title("Demonstrating - Requirement 9");
	std::cout << std::endl;
	std::cout << std::endl << " Provided MetricExecutive.h and MetricExecutive.cpp files in MetricExecutive package that enables collecting metrics";
	std::cout << std::endl << " for all the packages with names that match a specified pattern in a directory tree rooted at a specified path." << std::endl;

	StringHelper::title("Demonstrating - Requirement 10");
	std::cout << std::endl;
	std::cout << std::endl << " Automated unit test suite provided that exercises all the packages provided in submission and demonstrating that I met all requirements."<<std::endl;
	std::cout << std::endl << " To test on smaller no. of files, use Inputs folder provided in the project folder(it has all necessary test suites), set the path in run.bat" << std::endl << std::endl;
	std::cout << std::endl << "Press any key to proceed with source code analysis (This feature is provided to see all output one by one in case output is too long.)" << std::endl;
	_getch();
}

//----< method to demonstrate requirements 6 and 8 output>--------------------------------
void MetricExecutive::demonstrateReq68Output(SyntaxTree* treeRoot) {
	if (treeRoot != nullptr) {
		treeRoot->getComplexity(treeRoot->getRoot());
		StringHelper::Title("Demonstrating Requirement 6 - Abstract Sytax Tree", '=');
		std::cout << std::endl << "The Abstract Syntax Tree is: " << std::endl;
		treeRoot->treeWalk(treeRoot->getRoot());
		std::cout << std::endl << std::endl;
		StringHelper::Title("Demonstrating Requirement 8 - Metric Analysis", '=');
		std::cout << std::endl << "The complexity and line count analysis is: " << std::endl;
		MetricsAnalyzer *pMetricAnalyzer = new MetricsAnalyzer(treeRoot);
		pMetricAnalyzer->printHeader();
		pMetricAnalyzer->getAnalysis(treeRoot->getRoot());
		pMetricAnalyzer->printFooter();
		std::cout << "\n\n";
	}
}

//----< method to get files from the specified directory>--------------------------------
int MetricExecutive::getFiles(int argc, char* argv[]) {
	StringHelper::Title(" Searching for files", '=');
	if (argc < 2)
	{
		std::cout << std::endl << " Please enter the directory and pattern on command line to search for files.\n\n";
		return 1;
	}
	std::cout << std::endl << " Provided directory path to look for files is: " << argv[1];
	std::cout << std::endl << " Provided text pattern is: " << argv[2];
	StringHelper::Title("Following files were found", '-');
	FileMgr fileMgr(argv[1], dataStore);
	std::vector<char*> patternVec;
	char* patternArr = strtok(argv[2], ",");
	while (patternArr)
	{
		patternVec.push_back(patternArr);
		patternArr = strtok(NULL, ",");
	}
	for (size_t n = 0; n < patternVec.size(); ++n)
	{
		fileMgr.addPattern(patternVec[n]);
	}
	fileMgr.search();
	return 0;
}

//----< method to executer parsing on the found files>--------------------------------
void MetricExecutive::executeParsing() {
	for (auto fs : dataStore) {
		std::cout << std::endl;
		Util::Title("Analyzing file = " + fs, '=');
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try {
			if (pParser){
				if (!configure.Attach(fs)){
					std::cout << std::endl << "Could not open file " << fs << std::endl;
				}
			}
			else{
				std::cout << std::endl << " Parser not built" << std::endl;
			}
			// now that parser is built, use it
			while (pParser->next())
				pParser->parse();
			std::cout << std::endl;
		}
		catch (std::exception& ex){
			std::cout << std::endl << std::endl << ex.what() << std::endl << std::endl;
		}
		SyntaxTree* treeRoot = configure.getRepository()->getSyntaxTree();
		demonstrateReq68Output(treeRoot);
		std::cout << std::endl << "Press any key to see output of next file. (This feature is provided to see all output one by one in case output is too long.)" << std::endl;
		_getch();
	}
}
#ifdef TEST_METRIC
int main(int argc, char* argv[]) {
	MetricExecutive me;
	me.demonstrateReqs();
	if (me.getFiles(argc, argv)) {
		return 1;
	}
	me.executeParsing();
}

#endif