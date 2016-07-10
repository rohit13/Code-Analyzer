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
#pragma once
#include <iostream>
#include <string>
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/FileSystem.h"
#include "../DataStore/DataStore.h"
#include "../Parser/ConfigureParser.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"

//----< class to define public interfaces to oeprate metric executive>--------------------------------
class MetricExecutive {
private:
	DataStore dataStore;
public:
	MetricExecutive() {};
	int getFiles(int argc, char* argv[]);
	void demonstrateReqs();
	void demonstrateReq68Output(SyntaxTree* treeRoot);
	void executeParsing();
};