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
#include <sstream>
#include <iomanip>
#include <cstring>

#include "MDdataContainer.h"
#include "MDexception.h"

MDdataContainer::MDdataContainer()
:_data(nullptr), _size(0) {}

MDdataContainer::MDdataContainer(void *d, size_t s)
:_data((uint8_t*) d), _size(s) {}

MDdataContainer::MDdataContainer(const MDdataContainer &dc)
:_data(nullptr), _size(0) {
  *this = dc;
}

MDdataContainer::MDdataContainer(MDdataContainer &&dc)
:_data(nullptr), _size(0) {
  *this = std::move(dc);
}

MDdataContainer::~MDdataContainer() {}

uint8_t* MDdataContainer::GetDataPtr(unsigned int i) {
  if (i < _size)
    return &_data[i];

  return nullptr;
}

MDdataContainer& MDdataContainer::operator=(const MDdataContainer &dc) {
  if (&dc != this) {
    if (_data)
      delete _data;

    this->_data = new uint8_t[dc._size];
    memcpy(this->_data, dc._data, dc._size);
    this->_size  = dc._size;
  }

  return *this;
}

MDdataContainer& MDdataContainer::operator=(MDdataContainer &&dc) {
  if (&dc != this) {
    this->_data = dc._data;
    this->_size = dc._size;
    dc._data = nullptr;
    dc._size = 0;
  }

  return *this;
}

uint32_t* MDdataContainer::Get32bWordPtr(unsigned int i) {
  if ( (i*4) < _size )
    return (uint32_t*)&_data[i*4];

  stringstream ss;
  ss << "ERROR in MDdataContainer::Get32bWordPtr - the size is exceeded ";
  ss << "(i=" << i << " size=" << _size <<").";
  throw MDexception(ss.str());
}

size_t MDdataContainer::GetSize() {
  return _size;
}

void MDdataContainer::SetDataPtr(void *d, size_t aSize) {
  _data = (uint8_t*)d;
  _size = aSize;
}

void MDdataContainer::SetSize(size_t s) {
  _size = s;
}

void MDdataContainer::Dump() {
  if (_size%4) {
    cerr << " Not 32 bits data !! Trying to ignore \n" ;
    return;
  }

  unsigned int iword;
  unsigned int nword=_size>>2;

  for (iword=0 ; iword < nword ; ++iword) {
    cout << noshowbase << hex ;
    cout << setfill('0') << setw(8)
         << *Get32bWordPtr(iword) << "  ";
  }
  cout << endl;
  cout << dec << noshowbase << setfill(' ') ;
}


void MDdataWord::SetDataPtr(void *d, size_t aSize) {
  MDdataContainer::SetDataPtr(d);
  if ( _data ) {
    _size = aSize;
  }
}
