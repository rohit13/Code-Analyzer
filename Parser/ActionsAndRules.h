#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares parsing rules and corresponding   //
//						actions							           //
//  ver 2.2                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//	Author:		   Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu				   //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module defines several action classes.  Its classes provide
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable.

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
  parser.parse();                 // and parse it
  Repository(Scanner::Toker* pToker) //constructor for repository 
  ScopeStack<element*>& scopeStack() // get scope stack
  Scanner::Toker* Toker()			//get toker object
  size_t lineCount()				//get linecount

  Build Process:
  ==============
  Required files
	- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
	  ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
	  ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp,
	  MetricAnalysis.h, MetricAnalysis.cpp, SyntaxTree.h, SyntaxTree.cpp
  Build commands (either one)
	- devenv Project2_CodeAnalyzer.sln /rebuild debug

  Maintenance History:
  ====================
  ver 2.1 : 17 Mar 16
  - added Container, PrintContainer, PushContainer classes.
  - added FlowControlStructures, PrintFlowControlStructures, PushFlowControlStructures classes.
  - added LambdaDefinition, PrintLambdaDefinition, PushLambdaDefinition classes.
  - provided field to hold root of AST
  - provided interface to access AST
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
	of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/SyntaxTree.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
 * - chanage lineCount to two fields: lineCountStart and lineCountEnd
 */

 ///////////////////////////////////////////////////////////////
 // Repository instance is used to share resources
 // among all actions.
class Repository  // application specific
{
	ScopeStack<ASTNode*> stack;
	SyntaxTree* treeRoot;
	Scanner::Toker* p_Toker;
	ASTNode* gnsNode;
public:
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
		gnsNode = new ASTNode("GNS", "namespace", this->lineCount(), 0, 1, 0);
		scopeStack().push(gnsNode);
		treeRoot = new SyntaxTree(gnsNode);
	}

	//destructor
	~Repository() {
	}

	//return tree root
	SyntaxTree* getSyntaxTree() {
		return treeRoot;
	}
	//get node of the tree
	ASTNode* getNode() {
		return gnsNode;
	}
	//return stack
	ScopeStack<ASTNode*>& scopeStack(){
		return stack;
	}
	//return toker
	Scanner::Toker* Toker(){
		return p_Toker;
	}
	//return linecount
	size_t lineCount(){
		return (size_t)(p_Toker->currentLineCount());
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ASTNode* node = new ASTNode;
		node->setType("unknown");
		node->setName("anonymous");
		node->setLineCount(p_Repos->lineCount());
		p_Repos->scopeStack().top()->addChild(node);
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->scopeStack().size() == 0)
			return;
		ASTNode* node = p_Repos->scopeStack().pop();
		node->setStartLineCount(node->getLineCount() - 1);
		node->setEndLineCount(p_Repos->lineCount());
		node->setLineCount(node->getEndLineCount() - node->getStartLineCount());
		p_Repos->getNode()->setEndLineCount(p_Repos->lineCount());
		p_Repos->getNode()->setLineCount(node->getEndLineCount() - p_Repos->getNode()->getStartLineCount());
		if (node->getType() == "function" || node->getType() == "flow_control" || node->getType() == "class" || node->getType() == "namespace" || node->getType() == "struct")
		{
			std::cout << "\n  " << node->getType() << ", " << node->getName() << ", lines = " << node->getLineCount();
			std::cout << "\n";
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if ((pTc->find("#") < pTc->length()) && (pTc->find("#") <= 1))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
	Repository* p_Repos;
public:
	PrintPreproc(Repository* rep) {
		p_Repos = rep;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[] = { "for", "while", "switch", "if", "catch","do","try","else" };
		for (int i = 0; i < 8; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->getSyntaxTree()->deleteChild(p_Repos->getSyntaxTree()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		ASTNode* node = new ASTNode;
		node->setType("function");
		node->setName(name);
		node->setLineCount(p_Repos->lineCount());
		p_Repos->scopeStack().top()->addChild(node);
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
public:
	PrintFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  FuncDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		int len = static_cast<int>(pTc->find(")"));
		std::cout << "\n\n  Pretty Stmt:    ";
		for (int i = 0; i < len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
		  "const", "extern", "friend", "mutable", "signed", "static",
		  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
		  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i < numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end >= tc.length())
				end = tc.find(">::");
			if (end >= tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","else","try" };
		for (int i = 0; i < 7; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i < in.length(); ++i)
			tc.push_back(in[i]);
		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);
			// remove modifiers, comments, newlines, returns, and initializers
			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i) {
				if (isModifier(tc[i]))
					continue;
				if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
					continue;
				if (tc[i] == "=" || tc[i] == ";")
					break;
				else
					se.push_back(tc[i]);
			}
			if (se.length() == 2)  // type & name, so declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		// show cleaned semiExp
		std::cout << "\n  Declaration: " << se.show();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
		  "const", "extern", "friend", "mutable", "signed", "static",
		  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
		  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i < numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end >= tc.length())
				end = tc.find(">::");
			if (end >= tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
	}

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i < in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);
			// remove modifiers, comments, newlines, returns, and initializers
			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i]))
					continue;
				if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
					continue;
				if (tc[i] == "=" || tc[i] == ";")
					break;
				else
					se.push_back(tc[i]);
			}
			if (se.length() != 2)  // not a declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		}
		// show cleaned semiExp
		std::cout << "\n  Executable: " << se.show();
	}
};


///////////////////////////////////////////////////////////////
//rule to detect containers i.e. class, struct, namespace
class Container :public IRule {
	bool isSpecialKeyword(const std::string& tok) {
		const static std::string keys[] = { "class","struct","namespace" };
		for (std::string key : keys) {
			if (tok == key)
				return true;
		}
		return false;
	}
	bool doTest(ITokCollection*& ptrTC) {
		ITokCollection& tokC = *ptrTC;
		if (tokC.find("{") < tokC.length()) {
			for (int pos = 0; pos < tokC.find("{") - 1; pos++) {
				if (isSpecialKeyword(tokC[pos])) {
					doActions(ptrTC);
					return true;
				}
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
//class to print container i.e. class,struct,namespace.
class PrintContainer : public IAction {
	Repository* p_Repos;
public:
	PrintContainer(Repository* rep) {
		p_Repos = rep;
	}
	void doAction(ITokCollection*& ptrTc) {
		std::string container = ptrTc->show().c_str();
		if (container.find("class") < container.length()) {
			std::cout << "\n  Class Def: " << ptrTc->show().c_str();
		}
		else if (container.find("struct") < container.length()) {
			std::cout << "\n  Struct Def: " << ptrTc->show().c_str();
		}
		else if (container.find("namespace") < container.length()) {
			std::cout << "\n  Namespace Def: " << ptrTc->show().c_str();
		}
	}
};

///////////////////////////////////////////////////////////////
//action to push containers onto scopestack and in the tree.
class PushContainer :public IAction {
	Repository* p_Repos;
	bool isSpecialKeyword(const std::string& tok) {
		const static std::string keys[] = { "class","struct","namespace" };
		for (std::string key : keys) {
			if (tok == key)
				return true;
		}
		return false;
	}
public:
	PushContainer(Repository* rep) {
		p_Repos = rep;
	}
	void doAction(ITokCollection*& ptrTc) {
		//delete anonymous scope
		p_Repos->getSyntaxTree()->deleteChild(p_Repos->getSyntaxTree()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();
		std::string name = "";
		// push container scope
		for (int i = 0; i < ptrTc->find("{") - 1; i++) {
			if (isSpecialKeyword((*ptrTc)[i])) {
				name = (*ptrTc)[++i];
				break;
			}
		}
		ASTNode* node = new ASTNode;
		node->setName(name);
		std::string container = ptrTc->show().c_str();
		if (container.find("class") < container.length()) {
			node->setType("class");
		}
		else if (container.find("struct") < container.length()) {
			node->setType("struct");
		}
		else if (container.find("namespace") < container.length()) {
			node->setType("namespace");
		}
		node->setLineCount(p_Repos->lineCount());
		p_Repos->scopeStack().top()->addChild(node);
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
//rule to check for control structures like for,while,if,switch etc.
class FlowControlStructures : public IRule {
public:
	bool isSpecialKeyWord(const std::string& tok) {
		const static std::string keys[] = { "for","while","if","switch","else", "try","catch","do" };
		for (std::string key : keys) {
			if (tok == key)
				return true;
		}
		return false;
	}
	bool doTest(ITokCollection*& ptrTC) {
		ITokCollection& tokc = *ptrTC;
		if (tokc[tokc.length() - 1] == "{") {
			for (int i = 0; i < tokc.find("{"); i++) {
				if (isSpecialKeyWord(tokc[i])) {
					doActions(ptrTC);
					return true;
				}
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print flow control structures

class PrintFlowControlStructure : public IAction
{
	Repository* p_Repos;
public:
	PrintFlowControlStructure(Repository* rep) {
		p_Repos = rep;
	}
	void doAction(ITokCollection*& ptrTC) {
		std::cout << "\n  Flow control statement: " << ptrTC->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to push flow control structures in tree

class PushFlowControlStructure :public IAction {
	Repository* p_Repos;
public:
	PushFlowControlStructure(Repository* rep) {
		p_Repos = rep;
	}
	bool isSpecialKeyWord(const std::string& tok) {
		const static std::string keys[] = { "for","while","if","switch","else", "try","catch","do" };
		for (std::string key : keys) {
			if (tok == key)
				return true;
		}
		return false;
	}
	void doAction(ITokCollection*& ptrTC) {
		p_Repos->getSyntaxTree()->deleteChild(p_Repos->getSyntaxTree()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();
		int i;
		for (i = 0; i < ptrTC->length(); i++)
		{
			if (isSpecialKeyWord((*ptrTC)[i]))
			{
				break;
			}
		}
		std::string name = (*ptrTC)[i];
		ASTNode* node = new ASTNode;
		node->setType("flow_control");
		node->setName(name);
		node->setLineCount(p_Repos->lineCount());
		p_Repos->scopeStack().top()->addChild(node);
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// action to detect lambda definitions

class LambdaDefinition : public IRule
{
public:
	//Tests for Lambda
	bool doTest(ITokCollection*& ptrTC)
	{
		ITokCollection& tokC = *ptrTC;
		if (tokC[tokC.length() - 1] == "{") {
			size_t len = tokC.find("]");
			if ((len < tokC.length() && tokC[len + 1] == "(") || tokC[tokC.length() - 2] == "]") {
				doActions(ptrTC);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push lambda definitions on stack 

class PushLambda : public IAction
{
	Repository* p_Repos;
public:
	//Constructor
	PushLambda(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	//pushe lambda to the stack
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->getSyntaxTree()->deleteChild(p_Repos->getSyntaxTree()->getRoot(), p_Repos->scopeStack().top());
		p_Repos->scopeStack().pop();
		// push Lambda scope
		ASTNode* node = new ASTNode;
		node->setType("lambda");
		node->setName("lambdaScope");
		node->setStartLineCount(p_Repos->lineCount());
		p_Repos->scopeStack().top()->addChild(node);
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// action to Print Lambda definitions on the console

class PrintLambda : public IAction
{
	Repository* p_Repos;
public:
	//Constructor
	PrintLambda(Repository* rep)
	{
		p_Repos = rep;
	}
	//Prints Lambda
	void doAction(ITokCollection*& ptrTC)
	{
		ITokCollection& tokC = *ptrTC;
		size_t len = tokC.find("[");
		std::cout << "\n  Executable: \n  ";
		for (size_t i = 0; i < len - 2; i++)
			if (tokC[i] != "\n")
				std::cout << tokC[i];
		std::cout << "\n";
		std::cout << "\n  Lambda: \n  ";
		for (size_t i = len - 1; i < tokC.length(); i++)
			if (tokC[i] != "\n")
				std::cout << tokC[i];
		std::cout << "\n";
	}
};
#endif
