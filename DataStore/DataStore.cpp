/////////////////////////////////////////////////////////////////////
//  DataStore.cpp - cpp file for DataStore.h				       //
//  ver 2.0                                                        //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//	Application:   Project#2 - CSE 687-Object Oriented Design	   //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Author:        Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Defines DataStore class for storing files found in the directory structure

Public Interface:
=================
None. Uses test stub to test the DataStore.h files.

Build Process:
==============
Required files:
	DataStore.h
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 2.0 : 17 Mar 16
-changed documentation
ver 1.0 : 12 Feb 15
- first release
*/
#include "DataStore.h"
#include <iostream>

#ifdef TEST_DATASTORE

int main()
{
  std::cout << "\n  Testing DataStore";

  DataStore ds;
  ds.save("one");
  ds.save("two");
  ds.save("three");
  DataStore::iterator iter = ds.begin();
  std::cout << "\n  " << (*iter).c_str();

  for (auto item : ds)
  {
    std::cout << "\n  " << item.c_str();
  }
  std::cout << "\n\n";
}

#endif
