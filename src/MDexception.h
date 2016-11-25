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

#ifndef __MDEXCEPTION_H
#define __MDEXCEPTION_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <exception>

class MDexception {

 public:
  enum MD_SEVERITY
    {
      WARNING=0,
      SERIOUS=1,
      FATAL=2
    };

  MDexception(const std::string & aErrorDescription = "",MD_SEVERITY = SERIOUS );
  // Default destructor has nothing to do
  // except be declared virtual and non-throwing.
  ~MDexception() throw() {}
  std::string GetDescription() {return mErrorDescription;}

 protected: // sometimes derived classes want to manipulate this one
  std::string mErrorDescription;
  MD_SEVERITY mSeverity;

};

#endif
