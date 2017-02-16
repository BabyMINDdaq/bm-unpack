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

#include <iostream>


#include "MDfileManager.h"

using namespace std;

MDfileManager::~MDfileManager() {
  for(unsigned int i=0;i<_dateFiles.size();i++)
    delete _dateFiles[i];

  _dateFiles.resize(0);
}

// Scan the list given at the creation and add files or runs according to the recognized format
bool  MDfileManager::Open() {
  unsigned int myFileCount(0);
  stringstream toScan(_list,ios_base::in);
  string  strBuf;
  do {
    toScan >> strBuf ;
    if (IsFileName(strBuf)) {
      if ( AddFile(strBuf,_path) ) myFileCount++;
    }
    if (IsNumber(strBuf)) {
      myFileCount += AddRun(strBuf,_path);
    }
  } while (!toScan.eof());
  if (myFileCount )
    return true;

  cout << " No file open. Aborting." << endl;
  return false;
}

char* MDfileManager::GetNextEvent() {
  char* eventBuffer = _dateFiles[ _fileCount  ]->GetNextEvent();

  if( !eventBuffer){
    cout<<"++++ End of file "<< _dateFiles[ _fileCount  ]->GetFileName() << "  ++++" << endl;
    _dateFiles[ _fileCount  ]->Close();
    if( ++_fileCount<_dateFiles.size() ){
      cout<<"++++ Start of file "<< _dateFiles[ _fileCount  ]->GetFileName() << "  ++++" << endl;
      eventBuffer = _dateFiles[ _fileCount  ]->GetNextEvent();
    }
  }
  return eventBuffer;
}

bool MDfileManager::AddFile(string fn, string fp) {
  vector<string> SearchDirs = FillDataDir( fp );
  for( unsigned int di = 0; di < SearchDirs.size(); ++di ){
    string dir = SearchDirs[di];
    cout << "Trying to add file " << fn << " from " << dir << endl;

    MDdataFile* f = new MDdataFile(fn, dir);
    if ( f->Open() ){
      this->AddFile(f);
      cout << "File " << fn << " is added" << endl;
      return true;
    }
  }
  return false;
}

vector<string> MDfileManager::FillDataDir( string aDirs ) {
  vector<string> Dirs;
  stringstream toScan(aDirs,ios_base::in);
  string  strBuf;
  do {
    toScan >> strBuf ;
    Dirs.push_back( strBuf );
  } while (!toScan.eof());

  return Dirs;
}

unsigned int MDfileManager::AddRun( string aRun, string aDir ) {
  list<string> fileList;
  list<string>::iterator itl;
  stringstream ss(aRun,ios_base::out|ios_base::in );
  int runNum(0);
  // get run Number as int
  ss >> runNum;
  // Check
  if (ss.fail()) {
    cerr << "ERROR in MDfileManager::AddRun : The run number string " << aRun << " does not match an integer" << endl;
    return 0;
  }
  vector<string> SearchDirs = FillDataDir( aDir );
  unsigned int nFiles=0;

  for( unsigned int di = 0; di < SearchDirs.size(); ++di ){
    string dir = SearchDirs[di];
    cout << "Going to search for run " << aRun << " data files in " << dir << endl;
    DIR *dp;
    struct dirent *dirp;
    if( ( dp = opendir( dir.c_str() ) ) != NULL ) {
      while ( ( dirp = readdir( dp ) ) != NULL ) { // get the list of files in path
        fileList.push_back(string( dirp->d_name ));
      }
      fileList.sort(); // sort the list of files
      string SearchStr = aRun+".";
      for ( itl=fileList.begin(); itl!=fileList.end(); itl++ ) {
        string FileName = *itl;
        //cout << "Found a file : " << FileName << endl;
        // check if the file neme is correct and the run number matches
        if (IsFileName(FileName)) {
          float foundRunNum(0);
          ss.clear();
          ss.str(FileName);
          ss >> foundRunNum;
          //cout<<"found Run Num : "<< foundRunNum << endl;

          if ((int)foundRunNum == runNum){
            if( AddFile(FileName, dir) ) nFiles++;
          } else {
           //cout << "Rejected : " << foundRunNum << " != " << runNum << endl;
          }
        }
      }
    } else {
      cout << "Cannot open " << dir << endl;
    }
  }
  return nFiles;
}

bool MDfileManager::IsNumber( const std::string& s ) {
  for ( unsigned int i = 0; i < s.length(); i++) {
    if ( !std::isdigit( s[ i ] ) ) return false;
  }
  if (s.length()) return true;
  return false;
}

bool MDfileManager::IsFileName( const std::string& s ) {
  int length, size;
  size_t pos;
  size = s.size();
  if (size<3)
    return false;

  pos = s.find_last_of('.');
  if (pos == std::string::npos)
    return false;

  // check if the file name and the file extention are numeric
  char buffer[32];
  length = s.copy(buffer, pos, 0);
  buffer[length]='\0';
  string FilePrefix(buffer);
  //std::cout << "found run number : " << FilePrefix << std::endl;

  length = s.copy(buffer, (size - pos), (pos + 1));
  buffer[length]='\0';
  string FileSuffix(buffer);
  //cout << "found file index : " << FileSuffix << std::endl;
/*
  if (IsNumber(FilePrefix) && IsNumber(FileSuffix))
    return true;
*/
  if (IsNumber(FileSuffix)) 
    return true;

  return false;
}


