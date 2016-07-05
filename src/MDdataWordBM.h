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

#ifndef __MDDATWORD_BM_H
#define __MDDATWORD_BM_H

#include "MDdataWord.h"
// #include "MDTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

class MDdataWordBM : public MDdataWord {

 public:
  MDdataWordBM(void *d=0) : MDdataWord(d) {}
  virtual ~MDdataWordBM() {}

  void Dump();

  uint32_t GetDataType();
  uint32_t GetSid();
  uint32_t GetHeadTrailId();
  uint32_t GetBoardId();
  uint32_t GetSpillTag();
  uint32_t GetSpillTime();
  uint32_t GetChannelId();
  uint32_t GetHitTime();
  uint32_t GetHitId();
  uint32_t GetTagId();
  uint32_t GetHitCount();
  uint32_t GetEdgeId();
  uint32_t GetTriggerTime();
  uint32_t GetTriggerTag();
  uint32_t GetTriggerTagShort();
  uint32_t GetAmplitudeId();
  uint32_t GetAmplitude();
  uint32_t GetHumidity();
  uint32_t GetTemperature();

  enum DWBMDataType {
   SpillHeader   = 0x0,
   TrigHeader    = 0x1,
   TimeMeas      = 0x2,
   ChargeMeas    = 0x3,
   TrigTrailer1  = 0x4,
   TrigTrailer2  = 0x5,
   SpillTrailer1 = 0x6,
   SpillTrailer2 = 0x7
  };

  enum DWBMAmplitideId {
    Amplitide_HG = 2,
    Amplitide_LG = 3,
    Baseline     = 6
  };

 private:

  enum DWBMMask {
   DataTypeMask     = 0xF0000000,
   SidMask          = 0x000C0000,
   BoardIdMask      = 0x0FE00000,
   SpillTagMask     = 0x0000FFFF,
   SpillTimeMask    = 0x0FFFFFFF,
   ChannelIdMask    = 0x0FE00000,
   HitTimeMask      = 0x00000FFF,
   HitIdMask        = 0x001C0000,
   TagIdMask        = 0x00030000,
   HitCountMask     = 0x0F800000,
   HeadTrailIdMask  = 0x00100000,
   EdgeIdMask       = 0x00008000,
   TriggerTimeMask  = 0x000FFFFF,
   TriggerTagMask   = 0x0FFFFFFF,
   TrTagShortMask   = 0x00000003,
   AmplitudeIdMask  = 0x0000F000,
   AmplitudeMask    = 0x00000FFF,
   HumidityMask     = 0x000000FF,
   TemperatureMask  = 0x003FFF00
  };

  enum DWBMShift {
   DataTypeShift     = 28,
   SidShift          = 18,
   BoardIdShift      = 21,
   SpillTagShift     = 0,
   SpillTimeShift    = 0,
   ChannelIdShift    = 21,
   HitTimeShift      = 0,
   HitIdShift        = 18,
   TagIdShift        = 16,
   HitCountShift     = 23,
   HeadTrailIdShift  = 20,
   EdgeIdShift       = 15,
   TriggerTimeShift  = 0,
   TriggerTagShift   = 0,
   AmplitudeIdShift  = 12,
   AmplitudeShift    = 0,
   HumidityShift     = 0,
   TemperatureShift  = 8
  };
};

ostream &operator<<(ostream &s, MDdataWordBM &dw);

#endif

