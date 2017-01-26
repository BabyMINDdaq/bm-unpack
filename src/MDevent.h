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

#ifndef _MDEVENT_H_
#define _MDEVENT_H_ 1

#include "MDdataContainer.h"
#include "BMDDataHeaders.h"

class MDevent : public MDdataContainer {
public:
  MDevent() : MDdataContainer() {}
  MDevent(void *d, size_t s=1) : MDdataContainer(d, s) {}
  virtual ~MDevent() {}

  void SetDataPtr(void *d, size_t s=1) override;

  BMDEventHeader* HeaderPtr() {
    return reinterpret_cast<BMDEventHeader*>(_data);
  }

  uint8_t* PayLoadPtr() {
    return _data + sizeof(BMDEventHeader);
  }
};

#endif
