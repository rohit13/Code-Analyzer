#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////
//  FileMgr.h - find files matching specified patterns			   //
//				on a specified path								   //
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
Required files
Build commands (either one)
- devenv Project2_CodeAnalyzer.sln /rebuild debug

Maintenance History:
====================
 * ver 4.0 : 17 Mar 2016. Removed filepath comment.
 * ver 3.0 : 13 Mar 2016
 * ver 2.0 : 12 Mar 2016
 * - fixes bug in directory recursion in find(path)
 * - reduces scope of for loop in find for efficiency
 * ver 1.0 : 19 Feb 2016
 * - first release
 */
#include "FileSystem.h"
#include "../DataStore/DataStore.h"
#include <iostream>
 //----< class for declaring file manager methods>--------------------------------
class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;
  //----< FileManager constructor>--------------------------------
  FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds)
  {
    patterns_.push_back("*.*");
  }
  //----< method to add pattern (extension) to search for in directory>--------------------------------
  void addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  //----< method to invoke find method>--------------------------------
  void search()
  {
    find(path_);
  }
  //----< method to do search operation in the given directory>--------------------------------
  void find(const std::string& path)
  {
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    //---------------------------------------------------------
    // The statement above is not necessary, but helps to
    // make the processing clearer.  To show that, comment
    // it out and uncomment the following line.
    //
    // std::string fpath = path;
    //
    // Remove the std::cout statements below after you're 
    // convinced that everything works as it should.
    //---------------------------------------------------------

    //std::cout << "\n  ++ " << fpath;
    for (auto patt : patterns_)  // the scope of this for loop was
    {                            // changed to improve efficiency
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      for (auto f : files)
      {
        std::cout << "\n  --   " << f;
        store_.save(fpath+'\\'+f);
      }
    }
    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;  // here's the fix
      find(dpath);
    }
  }
private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
};

#endif
