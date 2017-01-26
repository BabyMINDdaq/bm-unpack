/** This file is part of BabyMINDdaq software package. This software
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
 *  \author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
 *            University of Geneva
 *
 *  \created  Jan 2017
 */

#ifndef BMD_DATA_HEADERS_H
#define BMD_DATA_HEADERS_H 1

#include <ctime>

struct BMDDataFragmenHeader {
  size_t size_;     // Make sure that the size_ is declared before any other data member.
};

struct BMDEventHeader {
  size_t size_;     // Make sure that the size_ is declared before any other data member.

  std::time_t  time_;
  uint32_t     event_id_;
  uint8_t      type_;
  uint8_t      n_subevents_;
  uint16_t     n_fragments_;

  bool isSuperEvent() {
    return static_cast<bool>(this->type_ & 0x80);
  }

  unsigned int  eventType() {
    return static_cast<int>(this->type_ & 0x7f);
  }
};

#endif


