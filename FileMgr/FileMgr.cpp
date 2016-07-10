/////////////////////////////////////////////////////////////////////
//  FileMgr.cpp - Cpp file for FileMgr.h							   //
//  ver 4.0                                                        //
//  Language:      C++, Visual Studio 2015						   //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//	Application:   Project#2 - CSE 687-Object Oriented Design	   //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Author:        Rohit Sharma, Syracuse University			   //
//                 (315) 935-1323, rshar102@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*Module Operations:
==================
find files matching specified patterns on a specified path

Public Interface:
=================
FileMgr(const std::string& path, DataStore& ds)
void addPattern(const std::string& patt)
void search()
void find(const std::string& path)

Build Process:
==============
Required files: DataStore.h, FileMgr.h
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
* ver 4.0 : 17 Mar 2016. Uses test stub to test FileMgr.h files
* ver 3.0 : 13 Mar 2016
* ver 2.0 : 12 Mar 2016
* - fixes bug in directory recursion in find(path)
* - reduces scope of for loop in find for efficiency
* ver 1.0 : 19 Feb 2016
* - first release
*/

#include "FileMgr.h"
#include "../DataStore/DataStore.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
  std::cout << "\n  Testing FileMgr";
  std::cout << "\n =================";

  DataStore ds;
  FileMgr fm("..", ds);
  fm.addPattern("*.h");
  fm.search();

  std::cout << "\n\n  contents of DataStore";
  std::cout << "\n -----------------------";
  for (auto fs : ds)
  {
    std::cout << "\n  " << fs;
  }
  std::cout << "\n\n";
  return 0;
}
#endif
