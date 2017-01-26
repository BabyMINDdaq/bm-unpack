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

#include "MDdataWordBM.h"

using namespace std;

uint32_t MDdataWordBM::GetDataType() {
  return ( (*(uint32_t*)(_data) & DataTypeMask ) >> DataTypeShift );
}

uint32_t MDdataWordBM::GetSid() {
  return ( (*(uint32_t*)(_data) & SidMask ) >> SidShift );
}

uint32_t MDdataWordBM::GetHeadTrailId() {
  return ( (*(uint32_t*)(_data) & HeadTrailIdMask ) >> HeadTrailIdShift );
}

uint32_t MDdataWordBM::GetBoardId() {
  return ( (*(uint32_t*)(_data) & BoardIdMask ) >> BoardIdShift );
}

uint32_t MDdataWordBM::GetSpillTag() {
  return ( (*(uint32_t*)(_data) & SpillTagMask ) >> SpillTagShift );
}

uint32_t MDdataWordBM::GetSpillTime() {
  return ( (*(uint32_t*)(_data) & SpillTimeMask ) >> SpillTimeShift );
}

uint32_t MDdataWordBM::GetChannelId() {
  return ( (*(uint32_t*)(_data) & ChannelIdMask ) >> ChannelIdShift );
}

uint32_t MDdataWordBM::GetHitTime() {
  return ( (*(uint32_t*)(_data) & HitTimeMask ) >> HitTimeShift );
}

uint32_t MDdataWordBM::GetHitId() {
  return ( (*(uint32_t*)(_data) & HitIdMask ) >> HitIdShift );
}

uint32_t MDdataWordBM::GetTagId() {
  return ( (*(uint32_t*)(_data) & TagIdMask ) >> TagIdShift );
}

uint32_t MDdataWordBM::GetHitCount() {
  return ( (*(uint32_t*)(_data) & HitCountMask ) >> HitCountShift );
}

uint32_t MDdataWordBM::GetEdgeId() {
  return ( (*(uint32_t*)(_data) & EdgeIdMask ) >> EdgeIdShift );
}

uint32_t MDdataWordBM::GetTriggerTime() {
  return ( (*(uint32_t*)(_data) & TriggerTimeMask ) >> TriggerTimeShift );
}

uint32_t MDdataWordBM::GetTriggerTag() {
  return ( (*(uint32_t*)(_data) & TriggerTagMask ) >> TriggerTagShift );
}

uint32_t MDdataWordBM::GetTriggerTagShort() {
  return ( (*(uint32_t*)(_data) & TrTagShortMask ) >> TriggerTagShift );
}

uint32_t MDdataWordBM::GetAmplitudeId() {
  return ( (*(uint32_t*)(_data) & AmplitudeIdMask ) >> AmplitudeIdShift );
}

uint32_t MDdataWordBM::GetAmplitude() {
  return ( (*(uint32_t*)(_data) & AmplitudeMask ) >> AmplitudeShift );
}

uint32_t MDdataWordBM::GetHumidity() {
  return ( (*(uint32_t*)(_data) & HumidityMask ) >> HumidityShift );
}

uint32_t MDdataWordBM::GetTemperature() {
  return ( (*(uint32_t*)(_data) & TemperatureMask ) >> TemperatureShift );
}

void MDdataWordBM::Dump() {
  cout << *this;
}

ostream & operator<<(ostream &s, MDdataWordBM &dw) {
  uint32_t dt= dw.GetDataType();
  s << " BM FEB ";
  switch (dt) {
  case MDdataWordBM::SpillHeader:
    s << "Spill Header  BoardId: " << dw.GetBoardId()
      << "  Sid: " << dw.GetSid()
      << "  Spill Tag: " << dw.GetSpillTag();
    break;

  case MDdataWordBM::TrigHeader:
    s << "Trigger Header  Gl. Trigger Tag: " << dw.GetTriggerTag()
      << " (" << dw.GetTriggerTagShort() << ")";
    break;

  case MDdataWordBM::TimeMeas:
    s << "Channel: " << dw.GetChannelId()
      << "  HitId: " << dw.GetHitId()
      << "  TagId: " << dw.GetTagId();
    if ( dw.GetEdgeId()==0 ) s << "  Time (0, RE): ";
    else s << "  Time (1, FE): ";

    s << dw.GetTriggerTime();
    break;

  case MDdataWordBM::ChargeMeas:
    s << "Channel: " << dw.GetChannelId()
      << "  HitId: " << dw.GetHitId()
      << "  TagId: " << dw.GetTagId()
      << "  Amplitude Id: " << dw.GetAmplitudeId()
      << "  Charge: " << dw.GetAmplitude();
    break;

  case MDdataWordBM::TrigTrailer1:
    s << "Trigger Trailer (1)  Gl. Trigger Tag: " << dw.GetTriggerTag();
    break;

  case MDdataWordBM::TrigTrailer2:
    s << "Trigger Trailer (2)  Hit count: " << dw.GetHitCount()
      << "  Trigger Time: " << dw.GetTriggerTime();
    break;

  case MDdataWordBM::SpillTrailer1:
    s << "Spill Trailer (1)  BoardId: " << dw.GetBoardId()
      << "  Sid: " << dw.GetSid();
    if (dw.GetHeadTrailId()==0)
      s << "  Spill Tag: " << dw.GetSpillTag();
    else
      s << "  Temperature: " << dw.GetTemperature() << "  Humidity: " << dw.GetHumidity();

    break;

  case MDdataWordBM::SpillTrailer2:
    s << "Spill Trailer (2)  Spill time: " << dw.GetSpillTime();
    break;

  default:
    s << "Unknown data word (" << hex << *(dw.GetDataPtr()) << dec << ")\n";
    break;
  }

  return s;
}