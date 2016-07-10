#ifndef METRICSANALYSIS_H
#define METRICSANALYSIS_H
#pragma once
/////////////////////////////////////////////////////////////////////
//  MetricsAnalysis.h - Prints the details of the analysis of AST  //
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
Required files: SyntaxTree.h
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release
*/


#include "../AST/SyntaxTree.h"
//----< class to define public interfaces required for metric analysis>--------------------------------
class MetricsAnalyzer
{
public:
	MetricsAnalyzer(SyntaxTree* ast) : pAST(ast){} //constructor
	void getAnalysis(ASTNode* currNode);	//print the analysis
	void printHeader(); //print header
	void printFooter(); //print footer
private:
	SyntaxTree* pAST;
	static const char separator = ' ';
	static const int numRange = 20;
	static const int nameRange = 35;
	static const int totalRange = 79;
};

#endif

