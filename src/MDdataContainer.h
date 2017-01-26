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

#ifndef __MDDATACONTAINER_H
#define __MDDATACONTAINER_H

#include <cstdint>

using namespace std;

class MDdataContainer {
protected:
  uint8_t  *_data;
  size_t    _size;

public:
  MDdataContainer();
  MDdataContainer(void *d, size_t s=1);
  MDdataContainer(const MDdataContainer &dc);
  MDdataContainer(MDdataContainer &&dc);

  virtual ~MDdataContainer();

  MDdataContainer& operator=(const MDdataContainer &dc);
  MDdataContainer& operator=(MDdataContainer &&dc);

  uint8_t* GetDataPtr(unsigned int i=0);
  uint32_t* Get32bWordPtr(unsigned int i=0);

  virtual void SetDataPtr( void *d, size_t aSize=1 );

  size_t GetSize();
  void SetSize(size_t s);

  virtual void Dump();
};

class MDdataWord : public MDdataContainer {
 public:
  MDdataWord(void *d=0) : MDdataContainer(d, 4) {}
  void SetDataPtr(void *d, size_t aSize=4) override;
};

#endif
