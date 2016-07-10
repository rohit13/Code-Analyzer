/////////////////////////////////////////////////////////////////////
//  SyntaxTree.h - Defines struct for Tree Node and provides       //
//					methods to walk the tree and manipulate it     //
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
This module defines a struct for Tree Node 
and provides methods to walk the tree and manipulate it 

Public Interface:
=================
struct ASTNode //struct node
void treeWalk(ASTNode* tNode); //method to walk the tree.
SyntaxTree(ASTNode* node) //constructor for syntax tree.
ASTNode* getRoot()//method to get the root of the tree.
void deleteChild(ASTNode* currentNode, ASTNode* deleteNode) //method to delete child nodes in a tree.
void getComplexity(ASTNode* currentNode) //method to getComplexity of each node

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
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <iostream>
#include <sstream>

//----< struct for defining AST node >--------------------------------
struct ASTNode
{
public:
	ASTNode() {}
	ASTNode(std::string name, std::string type, size_t startLineCount, size_t endLineCount, int lineCount, int complexity) {
		name_ =name;
		type_ = type;
		startLineCount_ = startLineCount;
		endLineCount_ = endLineCount;
		lineCount_ = lineCount;
		complexity_ = complexity;
	}
	std::string show()	//prints the details of the node
	{
		std::ostringstream temp;
		temp << "( Type: ";
		temp << type_;
		temp << ",  Name: ";
		temp << name_;
		temp << ",  LineCount: ";
		temp << lineCount_;
		temp << ", Complexity: ";
		temp << complexity_;
		temp << " )";
		return temp.str();
	}
	void addChild(ASTNode* cNode) { children_.push_back(cNode); }
	std::vector<ASTNode*>* getChild()	//returns the child of the node
	{
		return &children_;
	}
	std::string getName() {
		return name_;
	}
	void setName(std::string name) {
		name_ = name;
	}
	std::string getType() {
		return type_;
	}
	void setType(std::string type) {
		type_ = type;
	}
	size_t getLineCount() {
		return lineCount_;
	}
	void setLineCount(size_t lineCount) {
		lineCount_ = lineCount;
	}
	size_t getStartLineCount() {
		return startLineCount_;
	}
	void setStartLineCount(size_t startLineCount) {
		startLineCount_ = startLineCount;
	}
	size_t getEndLineCount() {
		return endLineCount_;
	}
	void setEndLineCount(size_t endLineCount) {
		endLineCount_ = endLineCount;
	}
	int getComplexity() {
		return complexity_;
	}
	void setComplexity(int complexity) {
		complexity_ = complexity;
	}
private:
	std::string name_;
	std::vector<ASTNode*> children_;
	std::string type_;
	size_t lineCount_;
	size_t startLineCount_;
	size_t endLineCount_;
	int complexity_=0;
};

//----< SyntaxTree class >--------------------------------
class SyntaxTree {		
private:
	ASTNode* rootNode;
public:
	SyntaxTree(ASTNode* node) : rootNode(node) {}
	void treeWalk(ASTNode* tNode);
	ASTNode* getRoot();
	void deleteChild(ASTNode* currentNode, ASTNode* deleteNode);
	void getComplexity(ASTNode* currentNode);
};