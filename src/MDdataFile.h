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

#ifndef __MDDATEFILE_H
#define __MDDATEFILE_H

#include <string>
#include <fstream>

class MDdateFile {
public:
  MDdateFile();
  MDdateFile(std::string fn, std::string fp=".");
  ~MDdateFile();

  void SetFileName(std::string fn) { _fileName = fn ;}
  std::string GetFileName()        { return _fileName;}

  void SetFilePath(std::string fp) { _filePath = fp ;}
  std::string GetFilePath()        { return _filePath;}

  bool  open();
  void  close();

  char* GetNextEvent();

  size_t GetStreamPos() {
    _curPos = _ifs.tellg();
    return _curPos;
  }

  void GoTo(size_t pos);
  void reset();

 private:
  std::ifstream _ifs;               // Input File Stream
  char         *_eventBuffer;
  std::string   _filePath;
  std::string   _fileName;
  size_t        _curPos;            // current stream position in file
  size_t        _fileSize;
  size_t        _eventSize;
  size_t        _nBytesRead;
};

#endif
