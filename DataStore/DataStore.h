#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////
//  DataStore.h - Defines DataStore class for storing files found  //
//				  in the directory structure					   //
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
void save(const std::string& filespec) 
void save(const std::string& filename, const std::string& path) 
iterator begin() 
iterator end() 

Build Process:
==============
Required files
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
ver 2.0 : 17 Mar 16
-changed documentation
ver 1.0 : 12 Feb 15
- first release
*/

#include <vector>
//----< class for declaring dataStore methods>--------------------------------
class DataStore
{
public:
  using iterator = std::vector<std::string>::iterator;
  void save(const std::string& filespec) { store.push_back(filespec);  }// method to save files in datastore given spec
  void save(const std::string& filename, const std::string& path) {} //method to save files in datastore given filename and path
  iterator begin() { return store.begin(); } //iterator begin method
  iterator end() { return store.end(); } //iterator end method
private:
  std::vector<std::string> store;
};
#endif
