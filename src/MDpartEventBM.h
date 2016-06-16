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

#ifndef __MDPARTEVENT_BM_H
#define __MDPARTEVENT_BM_H

#include "MDdataContainer.h"

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>

#define BM_FEB_NCHANNELS 96

class MDpartEventBM : public MDdataContainer {

 public:

  MDpartEventBM( void *d = 0 );
  virtual ~MDpartEventBM(){}


  void SetDataPtr( void *d, uint32_t aSize=0 );
//   void Dump();
  void Init();

  unsigned int GetTriggerTime() {return _triggerTime;}
  unsigned int GetHitCount()    {return _hitCount;}
  unsigned int GetNLeadingEdgeHits(unsigned int ich)  {return _nLeadingEdgeHits[ich];}
  unsigned int GetNTrailingEdgeHits(unsigned int ich) {return _nTrailingEdgeHits[ich];}
  unsigned int GetHitTime(unsigned int ih, unsigned int ich, char t);
  unsigned int GetLeadingTime(unsigned int ih, unsigned int ich)  { return GetHitTime(ih, ich, 'l');}
  unsigned int GetTrailingTime(unsigned int ih, unsigned int ich) { return GetHitTime(ih, ich, 't');}
  unsigned int GetHitAmplitude(unsigned int ich, char t);
  unsigned int GetTriggerTag() {return _triggerTag;}

  std::vector<unsigned int> GetLeadingTimes(unsigned int ih)   {return _leadingEdgeHit[ih];}
  std::vector<unsigned int> GetTrailingTimes(unsigned int ih)  {return _trailingEdgeHit[ih];}

  unsigned int getNumDataWords() {return _nDataWords;}

 private:
  unsigned int _triggerTime;
  unsigned int _hitCount;
  unsigned int _triggerTag;

  unsigned int          _nLeadingEdgeHits[BM_FEB_NCHANNELS];  /** Number leading edge hits per channel. */
  unsigned int          _nTrailingEdgeHits[BM_FEB_NCHANNELS]; /** Number trailing edge hits per channell.*/

  unsigned int          _amplitudeHits[BM_FEB_NCHANNELS][2];

  std::vector<unsigned int>  _leadingEdgeHit[BM_FEB_NCHANNELS];      /// A vector of leading edge hits per channel
  std::vector<unsigned int>  _trailingEdgeHit[BM_FEB_NCHANNELS];     /// A vector of trailing edge hits per channel

  unsigned int _nDataWords;
};

// ostream &operator<<(std::ostream &s,MDpartEventBM &df);

#endif
