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

#ifndef __MDFRAGMENT_BM_H
#define __MDFRAGMENT_BM_H

#include <vector>

#include "MDdataContainer.h"
#include "MDpartEventBM.h"
#include "BMDDataHeaders.h"

class MDfragmentBM : public MDdataContainer {
public:
  MDfragmentBM() : MDdataContainer() {}
  MDfragmentBM(void *d, size_t s=1) : MDdataContainer(d, s) {}

  virtual ~MDfragmentBM() { this->Clean(); }

  void SetDataPtr(void *d, size_t s=0) override;

  BMDDataFragmenHeader* HeaderPtr() {
    return reinterpret_cast<BMDDataFragmenHeader*>(_data);
  }

  uint8_t* PayLoadPtr() {
    return _data + sizeof(BMDDataFragmenHeader);
  }

  void Dump() override;
  void Init();
  void Clean();

  unsigned int GetBoardId()     {return _boardId;}
  unsigned int GetSpillTag()    {return _spillTag;}
  unsigned int GetHumidity()    {return _humidity;}
  unsigned int GetTemperature() {return _temperature;}

  unsigned int   GetNumOfTriggers() {return _trigEvents.size();}
  MDpartEventBM* GetTriggerEventPtr(unsigned int evId);

 private:
  unsigned int _boardId;
  unsigned int _spillTag;
  unsigned int _humidity;
  unsigned int _temperature;

  std::vector <MDpartEventBM*> _trigEvents;
};

// ostream &operator<<(std::ostream &s, MDfragmentBM &df);

#endif
