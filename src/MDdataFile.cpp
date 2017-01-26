/* This file is part of MAUS: http://micewww.pp.rl.ac.uk:8080/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>

#include "MDdataFile.h"
#include "MDdataWordBM.h"
#include "MDexception.h"

using namespace std;
MDdateFile::MDdateFile()
:_eventBuffer(nullptr),_filePath("."), _fileName("0.000"),
 _curPos(0),_fileSize(0), _eventSize(0), _nBytesRead(0) {}

MDdateFile::MDdateFile(string fn, string fp)
:_eventBuffer(nullptr),_filePath(fp), _fileName(fn),
 _curPos(0),_fileSize(0), _eventSize(0), _nBytesRead(0) {}

MDdateFile::~MDdateFile() {
  if (_eventBuffer)
    delete _eventBuffer;
}

bool MDdateFile::open() {

  string fullName = _filePath + "/" + _fileName;

  _ifs.open( fullName.c_str() );
  if ( _ifs.fail() ) {
    cerr << "Can not open file " << fullName.c_str() << endl;
    return false;
  }
  uint32_t end;
  _curPos = _ifs.tellg();
  _ifs.seekg (0, ios::end);
  end = _ifs.tellg();
  _fileSize = end - _curPos;
  //  cout << " File size " << _fileSize << endl;
  if ( _fileSize%4 != 0 ) {
    cerr << " File size is not a multiple of 4. The file " << fullName.c_str() << " is rejected!" << endl;
    return false;
  }
  _ifs.seekg (0, ios::beg); // go back to the begining ( = _curPos )
  return true;
}

void MDdateFile::close() {
  if (_eventBuffer)
    delete _eventBuffer;

  _eventBuffer = nullptr;
  _ifs.close();
}

char* MDdateFile::GetNextEvent() {
  _curPos = _ifs.tellg();
  if ( _nBytesRead < _fileSize ) {
    delete _eventBuffer;
    _eventBuffer = nullptr;

    if  ( !_ifs.read( ( char* ) &_eventSize, sizeof( _eventSize ) ) ) { // read event size
      cerr << "Unexpected End of File while trying to read event Size" << endl;
      return nullptr;
    } else { // read OK, go back to previous position
      _ifs.seekg (_curPos , ios::beg);
      _nBytesRead += sizeof( _eventSize );
      cout << " Event size " << _eventSize << endl;
      if ( _eventSize ) { // allocate memory for the event
        _eventBuffer = new char[_eventSize];
        if ( _eventBuffer == nullptr )  {
          cerr << "Memory allocation failed in MDdateFile::GetNextEvent()" << endl;
          return nullptr;
        }
        if  ( !_ifs.read( (char*)_eventBuffer, _eventSize ) ) { // read full DAQ event
          cerr << "Unexpected End of File while trying to read event" << endl;
          return nullptr;
        } else { // read OK
          _nBytesRead += _eventSize;
          return _eventBuffer;
        }
      } else {
        return nullptr;
      }
    }
  } else {
    return nullptr;
  }
}

void MDdateFile::GoTo(size_t pos) {
  _ifs.seekg (pos , ios::beg);
}

void MDdateFile::reset() {
  /* go back to the begining */
  _nBytesRead = 0;
  _ifs.clear();
  _ifs.seekg (0, ios::beg);
}
