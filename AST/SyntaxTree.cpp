/////////////////////////////////////////////////////////////////////
//  SyntaxTree.cpp - Defines methods to walk the tree			   //
//					 and manipulate it							   //
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
This module provides methods to walk the tree and manipulate it

Public Interface:
=================
void treeWalk(ASTNode* tNode); //method to walk the tree.
ASTNode* getRoot()//method to get the root of the tree.
void deleteChild(ASTNode* currentNode, ASTNode* deleteNode) //method to delete child nodes in a tree.
void getComplexity(ASTNode* currentNode) //method to getComplexity of each node

Build Process:
==============
Required files:
	SyntaxTree.h
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release
*/
#include "SyntaxTree.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <iostream>

//----< Method to traverse the tree >--------------------------------
void SyntaxTree::treeWalk(ASTNode * tNode)
{
	static size_t indentation = 0;
	std::cout << "\n " << std::string(5*indentation,' ')<<tNode->show();
	auto iterator = tNode->getChild()->begin();
	++indentation;
	while (iterator != tNode->getChild()->end()) {
		treeWalk(*iterator);
		++iterator;
	}
	--indentation;
}

//----< Method to get the root of the tree>--------------------------------
ASTNode * SyntaxTree::getRoot()
{
	return rootNode;
}

//----< Method to delete the child nodes of the given node >--------------------------------
void SyntaxTree::deleteChild(ASTNode * currentNode, ASTNode * deleteNode)
{
	if (deleteNode == nullptr || currentNode == nullptr || currentNode->getChild() == nullptr)
		return;
	else {
		std::vector<ASTNode*> *children = currentNode->getChild();
		std::vector<ASTNode*>::iterator iterator;
		for (iterator = children->begin(); iterator != children->end();) {
			if (deleteNode == (*iterator)) {
				delete * iterator;
				iterator = children->erase(iterator);
			}
			else {
				deleteChild((*iterator),deleteNode);
				++iterator;
			}
		}
	}
}

//----< Method to get the complexity of the construct i.e. node>--------------------------------
void SyntaxTree::getComplexity(ASTNode * currentNode)
{
	currentNode->setComplexity(currentNode->getComplexity() + 1);
	std::vector<ASTNode*>::iterator iterator;
	std::vector<ASTNode*> *children = currentNode->getChild();
	for (iterator = children->begin(); iterator != children->end(); iterator++) {
		getComplexity(*iterator);
	}
	for (iterator = children->begin(); iterator != children->end(); iterator++) {
		currentNode->setComplexity(currentNode->getComplexity()+(*iterator)->getComplexity());
	}
}


#ifdef SYNTAX_TREE
int main()
{
	title("Demonstrate Tree Traversal Strategies", '=');
	putLine();

	// build ad hoc tree

	sPtr pRoot = sPtr(new ASTNode("root"));
	sPtr pChild1 = sPtr(new ASTNode("child1"));
	pRoot->addChild(pChild1);
	sPtr pChild2 = sPtr(new ASTNode("child2"));
	pRoot->addChild(pChild2);
	sPtr pGrandChild11 = sPtr(new ASTNode("grandChild11"));
	pChild1->addChild(pGrandChild11);
	sPtr pGreatGrandChild11 = sPtr(new ASTNode("greatGrandChild11"));
	pGrandChild11->addChild(pGreatGrandChild11);
	sPtr pGrandChild21 = sPtr(new ASTNode("grandChild21"));
	pChild2->addChild(pGrandChild21);
	sPtr pGrandChild22 = sPtr(new ASTNode("grandChild22"));
	pChild2->addChild(pGrandChild22);

	// now start walking

	title("Recursive left-to-right Depth First Search");
	DFS_r_ltr(pRoot);
	putLine();

	title("Recursive bottom-to-top Depth First Search");
	DFS_r_ltrbtt(pRoot);
	putLine();

	title("Recursive right-to-left Depth First Search");
	DFS_r_rtl(pRoot);
	putLine();

	title("Stack based right-to-left Depth First Search");
	DFS_s_rtl(pRoot);
	putLine();

	title("Stack based left-to-right Depth First Search");
	DFS_s_ltr(pRoot);
	putLine();

	title("Queue based Breadth First Search");
	BFSq(pRoot);
	putLine(2);
}
#endif
