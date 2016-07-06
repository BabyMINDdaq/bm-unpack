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

#include "MDdataFile.h"
#include "MDdataWordBM.h"
#include "MDexception.h"

using namespace std;

MDdateFile::MDdateFile(string fn, string fp)
:_eventBuffer(NULL),_filePath(fp), _fileName(fn),_curPos(0)
,_fileSize(0),_nBytesRead(0), _lastSpill(-1) {}

MDdateFile::~MDdateFile() {
  if (_eventBuffer)
    free(_eventBuffer);
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
//   if (_eventBuffer)
//     delete _eventBuffer;

  _eventBuffer = 0;
  _ifs.close();
}


void MDdateFile::init() {
  this->reset();
  _curPos = _ifs.tellg();
  if (_eventBuffer)
    delete _eventBuffer;

  _eventBuffer = new char[4];

  while (!_ifs.eof()) {
//     _ifs.read( _eventBuffer, sizeof( _eventBuffer ) );
    _ifs.read( _eventBuffer, 4 );
    MDdataWordBM dw(_eventBuffer);
//     cout << dw << endl;
    if (dw.GetDataType() == MDdataWordBM::SpillHeader) {
      _curPos = _ifs.tellg();
//        cout << dw << endl;
//        cout << "pos: " << _curPos << endl;
      if (_spill_pos.size()) {
//         uint32_t size = _curPos - _spill_pos.back() - 4;
//         cout << "size: " << size << endl;
        _spill_size.push_back(_curPos - _spill_pos.back() - 4);
      }

      _spill_pos.push_back(_curPos-4);
    }
  }

  this->reset();
}

char* MDdateFile::GetNextEvent() {

  if ( (unsigned int)(++_lastSpill) >= _spill_size.size() )
   return NULL;

  uint32_t spillSize = _spill_size[_lastSpill];
  uint32_t spillPos  = _spill_pos[_lastSpill];
  cout << "GetNextEvent  pos: " << spillPos/4 << "  size: " << spillSize/4 
       << " in DW units (4 bytes)" << endl;
  return GetSpill(spillPos, spillSize);
}

void MDdateFile::GoTo(uint32_t pos) {
  _ifs.seekg (pos , ios::beg);
}

char* MDdateFile::GetSpill(uint32_t pos, uint32_t size) {
  _ifs.seekg (pos , ios::beg);

  if (_eventBuffer) delete _eventBuffer;
    _eventBuffer = new char[size];

  if ( !_ifs.read(_eventBuffer, size ) ) { // read full Spill event
    throw MDexception("Unexpected End of File while trying to read event");
//     cerr << "Unexpected End of File while trying to read event" << endl;
//     cerr << _ifs.gcount() << " bits read.\n";
//     return NULL;
  }

  return _eventBuffer;
}

void MDdateFile::reset() {
  /* go back to the begining */
  _nBytesRead = 0;
  _ifs.clear();
  _ifs.seekg (0, ios::beg);
  _lastSpill = -1;
}
