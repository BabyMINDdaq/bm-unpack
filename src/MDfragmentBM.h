/* This file is part of BabyMINDupack
 *
 * BabyMINDupack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BabyMINDupack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BabyMINDupack.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __MDFRAGMENT_BM_H
#define __MDFRAGMENT_BM_H

#include "MDdataContainer.h"

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>


class MDfragmentBM : public MDdataContainer {

 public:

  MDfragmentBM( void *d = 0 ) : MDdataContainer(d) {}
  virtual ~MDfragmentBM() {}

  void SetDataPtr( void *d, uint32_t aSize=0 );
  void Dump();
  void Init();

  unsigned int GetBoardId()     {return _boardId;}
  unsigned int GetSpillTag()    {return _spillTag;}
  unsigned int GetHumidity()    {return _humidity;}
  unsigned int GetTemperature() {return _temperature;}

  unsigned int GetNumOfTriggers() {return _trigEvents.size();}
  uint32_t*    GetTriggerEventPtr(unsigned int evId);

 private:
  unsigned int _boardId;
  unsigned int _spillTag;
  unsigned int _humidity;
  unsigned int _temperature;

  std::vector <unsigned int> _trigEvents;
};

ostream &operator<<(std::ostream &s,MDfragmentBM &df);

#endif