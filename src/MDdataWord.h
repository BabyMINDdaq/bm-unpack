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

#ifndef __MDDATAWORD_H
#define __MDDATAWORD_H

#include <stdint.h>

#include "MDdataContainer.h"

using namespace std;

class MDdataWord : public MDdataContainer {
 public:
  MDdataWord(void *d=0) : MDdataContainer(d, 4) { if (_data) _valid = true;}

  MDdataWord(MDdataWord& dw) {
    _data  = dw.GetDataPtr();
    _size  = dw.GetSize();
    _valid = dw.IsValid();
  }

  MDdataWord(const MDdataWord& dw) {}

  virtual ~MDdataWord(){}

  virtual void SetDataPtr(void *d, uint32_t aSize=4) {
    MDdataContainer::SetDataPtr(d);
    if ( _data ) {
      _size = aSize;
      _valid = true;
    }
  }

  virtual void Dump() {
    if (_valid)
      MDdataContainer::Dump();
    else cout << "  Data Word NOT valid " << endl;
  }
};

#endif
