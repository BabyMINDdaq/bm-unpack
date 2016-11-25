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

#ifndef __MDPARTEVENT_BM_H
#define __MDPARTEVENT_BM_H


#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "MDdataContainer.h"
#include "MDdataWordBM.h"

#define BM_FEB_NCHANNELS 96

class MDpartEventBM : public MDdataContainer {

 public:

  MDpartEventBM(void *d = 0);
  virtual ~MDpartEventBM() {}


  void SetDataPtr(void *d, uint32_t aSize=0);
  void Dump();
  void Init();

  void AddTimeHit(MDdataWordBM &dw);
  void AddAmplitudeHit(MDdataWordBM &dw);

  unsigned int GetTriggerTime() { return _triggerTime; }
  unsigned int GetHitCount()    { return _hitCount; }
  unsigned int GetNLeadingEdgeHits(unsigned int ich)  { return _nLeadingEdgeHits[ich]; }
  unsigned int GetNTrailingEdgeHits(unsigned int ich) { return _nTrailingEdgeHits[ich]; }
  unsigned int GetHitTime(unsigned int ih, unsigned int ich, char t);
  unsigned int GetHitTimeId(unsigned int ih, unsigned int ich, char t);

  unsigned int GetLeadingTime(unsigned int ih, unsigned int ich)    { return GetHitTime(ih, ich, 'l'); }
  unsigned int GetTrailingTime(unsigned int ih, unsigned int ich)   { return GetHitTime(ih, ich, 't'); }
  unsigned int GetLeadingTimeId(unsigned int ih, unsigned int ich)  { return GetHitTimeId(ih, ich, 'l'); }
  unsigned int GetTrailingTimeId(unsigned int ih, unsigned int ich) { return GetHitTimeId(ih, ich, 't'); }

  unsigned int GetHitAmplitude(unsigned int ich, char t);
  unsigned int GetHitAmplitudeId(unsigned int ich, char t);
  bool         LGAmplitudeHitExists(unsigned int ich) {return _lgHit[ich];}
  bool         HGAmplitudeHitExists(unsigned int ich) {return _hgHit[ich];}


  unsigned int GetTriggerTag()   {return _triggerTag;}
  unsigned int GetTriggerTagId() {return _triggerTagId;}

  std::vector<unsigned int> GetLeadingTimes(unsigned int ich)   { return _leadingEdgeHitTime[ich]; }
  std::vector<unsigned int> GetTrailingTimes(unsigned int ich)  { return _trailingEdgeHitTime[ich]; }

  unsigned int getNumDataWords() {return _nDataWords;}

  void SetTriggerEvents(std::vector <MDpartEventBM*> *te) {_trigEvents = te;}

 private:
  unsigned int _triggerTime;
  unsigned int _hitCount;
  unsigned int _triggerTag;
  unsigned int _triggerTagId;

  bool _lgHit[BM_FEB_NCHANNELS];
  bool _hgHit[BM_FEB_NCHANNELS];

  unsigned int _lgHitAmplitude[BM_FEB_NCHANNELS];
  unsigned int _hgHitAmplitude[BM_FEB_NCHANNELS];
  unsigned int _lgHitAmplitudeId[BM_FEB_NCHANNELS];
  unsigned int _hgHitAmplitudeId[BM_FEB_NCHANNELS];

  unsigned int _nLeadingEdgeHits[BM_FEB_NCHANNELS];  /** Number of leading edge hits per channel. */
  unsigned int _nTrailingEdgeHits[BM_FEB_NCHANNELS]; /** Number of trailing edge hits per channell.*/

  std::vector<unsigned int>  _leadingEdgeHitTime[BM_FEB_NCHANNELS];      /// A vector of leading edge hit timess per channel
  std::vector<unsigned int>  _trailingEdgeHitTime[BM_FEB_NCHANNELS];     /// A vector of trailing edge hit times per channel
  std::vector<unsigned int>  _leadingEdgeHitId[BM_FEB_NCHANNELS];        /// A vector of leading edge hit ids per channel
  std::vector<unsigned int>  _trailingEdgeHitId[BM_FEB_NCHANNELS];       /// A vector of trailing edge hit ids per channel

  unsigned int _nDataWords;

  std::vector <MDpartEventBM*> *_trigEvents;

  friend ostream &operator<<(std::ostream &s, MDpartEventBM &df);
};

#endif
