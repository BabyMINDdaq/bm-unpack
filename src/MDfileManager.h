/* This file is part of BabyMINDdaq software package. This software
 * package is designed for internal use for the Baby MIND detector
 * collaboration and is tailored for this use primarily.
 *
 * BabyMINDdaq is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BabyMINDdaq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BabyMINDdaq.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __MDFILEMANAGER_H
#define __MDFILEMANAGER_H

#include "MDevent.h"
#include "MDdataFile.h"

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <list>

class MDfileManager {
 public:
  /// Creator accepts a space seperated mixed list of files and run number as first argument
  /// and a space seperated list of of pathes as second argument
  MDfileManager(string aListOfFilesAndRuns="", string aPath=".")
  :_fileCount(0), _list(aListOfFilesAndRuns), _path(aPath) {};

  ~MDfileManager();

  char* GetNextEvent();

  void AddFile(MDdataFile *f) { _dateFiles.push_back( f ); }
  bool AddFile(string fn, string fp=".");
  unsigned int AddRun(string rn, string rp=".");

  void SetList(string aListOfFilesAndRuns) {_list = aListOfFilesAndRuns;}
  std::string GetList()   {return _list;}

  void SetPath(string fp) {_path = fp ;}
  std::string GetPath()   {return _path;}

  unsigned int GetNFiles()    {return _dateFiles.size();}
  MDdataFile*  GetFile(int i) {return _dateFiles[i];}
  vector<MDdataFile*>  GetFileVector() {return _dateFiles;}

  bool Open();

 private:
  vector<string> FillDataDir( string aDirs );
  bool IsNumber( const std::string& s );   // all characters are digits
  bool IsFileName( const std::string& s ); // Any string matching *.* is interpreted as a file name

  vector< MDdataFile* > _dateFiles;
  unsigned int          _fileCount;
  std::string           _list;
  std::string           _path;
};

#endif
