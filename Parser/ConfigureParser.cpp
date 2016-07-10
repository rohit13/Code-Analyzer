/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers              //
//  ver 2.2                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Project#2 - CSE 687-Object Oriented Design      //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//	Author:		   Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu				   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParseToConsole conConfig;
conConfig.Build();
conConfig.Attach(someFileName);

Build Process:
==============
Required files
- ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
ActionsAndRules.h, ActionsAndRules.cpp,
SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv Project1HelpS06.sln
- cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 2.2 : 17 Mar 16
-	Added getRepository public interface and PrintContainer,PushContainer,FlowControlStructures,
PrintFlowControlStructure,  PushFlowControlStructure,  LambdaDefinition,  PrintLambda,  PushLambda declarations.
ver 2.1 : 19 Feb 16
- Added PrintFunction action to FunctionDefinition rule
ver 2.0 : 01 Jun 11
- Major revisions to begin building a strong code analyzer
ver 1.1 : 01 Feb 06
- cosmetic changes to ConfigureParser.cpp
ver 1.0 : 12 Jan 06
- first release

*/
#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated
  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pContainer;
  delete pPrintContainer;
  delete pPushContainer;
  delete pFlowControlStructures;
  delete pPrintFlowControlStructure;
  delete pPushFlowControlStructure;
  delete pLambdaDefinition;
  delete pPrintLambda;
  delete pPushLambda;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);

    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false

    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pPrintFunction = new PrintFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
    pFunctionDefinition->addAction(pPrintFunction);
    pParser->addRule(pFunctionDefinition);

    // configure to detect and act on declarations and Executables

    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);

	//add container rule and correpsonding action to parser while building it
	pContainer = new Container;
	pPrintContainer = new PrintContainer(pRepo);
	pContainer->addAction(pPrintContainer);
	pPushContainer = new PushContainer(pRepo);
	pContainer->addAction(pPushContainer);
	pParser->addRule(pContainer);

	//add FlowControlStructure rule and correpsonding action to parser while building it
	pFlowControlStructures = new FlowControlStructures;
	pPrintFlowControlStructure = new PrintFlowControlStructure(pRepo);
	pFlowControlStructures->addAction(pPrintFlowControlStructure);
	pPushFlowControlStructure = new PushFlowControlStructure(pRepo);
	pFlowControlStructures->addAction(pPushFlowControlStructure);
	pParser->addRule(pFlowControlStructures);

	//add LambdaDefinition rule and correpsonding action to parser while building it
	pLambdaDefinition = new LambdaDefinition;
	pPrintLambda = new PrintLambda(pRepo);
	pLambdaDefinition->addAction(pPrintLambda);
	pPushLambda = new PushLambda(pRepo);
	pLambdaDefinition->addAction(pPushLambda);
	pParser->addRule(pLambdaDefinition);

    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

Repository * ConfigParseToConsole::getRepository()
{
	return pRepo;
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
