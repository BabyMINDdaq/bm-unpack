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

#include "MDfragmentBM.h"
#include "MDdataWordBM.h"

using namespace std;

void MDfragmentBM::SetDataPtr( void *d, uint32_t aSize ) {
  MDdataContainer::SetDataPtr(d);
  this->Init();
}

void MDfragmentBM::Init() {
//     cout << " Calling MDfragmentBM::Init() " << endl;
  this->UnValidate();

  _trigEvents.resize(0);

  unsigned int * ptr = this->Get32bWordPtr(0);
  MDdataWordBM dw(ptr);
  if ( dw.IsValid() ) {
    // Check the reliability of the header and decode the header information.
    if (dw.GetDataType() != MDdataWordBM::SpillHeader ){ // The data doesn't start with a header
      cout << dw << endl;
      throw MDexception("ERROR in MDfragmentBM::Init() : 1st word is not a spill header.");
    } else {
      _spillTag = dw.GetSpillTag();
      _boardId = dw.GetBoardId();
      bool done(false);
      int nDW(1);

      while (!done) {
        dw.SetDataPtr(++ptr);
        ++nDW;

        if (dw.GetDataType() == MDdataWordBM::TrigHeader) {
          _trigEvents.push_back(nDW-1);
        }

        if (dw.GetDataType() == MDdataWordBM::SpillTrailer1)
          done = true;
      }

      if ( dw.GetHeadTrailId() ||
           dw.GetBoardId()  != _boardId ||
           dw.GetSpillTag() != _spillTag) {
        stringstream ss;
        ss << "ERROR in MDfragmentBM::Init() : The spill trailer 1 is not consistent.\n";
        ss << "  Board Id: " <<  dw.GetBoardId() << "  ctrl bit: " << dw.GetEdgeId()
           << "  Tag: " << dw.GetSpillTag() << " (" << _spillTag << ")";
        throw MDexception( ss.str() );
      }

      dw.SetDataPtr(++ptr);
      ++nDW;

      if ( dw.GetDataType() != MDdataWordBM::SpillTrailer1 ||
           dw.GetHeadTrailId()   !=  1 ||
           dw.GetBoardId()  != _boardId ) {
        throw MDexception("ERROR in MDfragmentBM::Init() : The spill trailer 1 is not consistent.");
      }
      _temperature = dw.GetTemperature();
      _humidity = dw.GetHumidity();

      dw.SetDataPtr(++ptr);
      ++nDW;

      if ( dw.GetDataType() != MDdataWordBM::SpillTrailer2 )
        throw MDexception("ERROR in MDfragmentBM::Init() : The spill trailer 2 is not consistent.");

      _size = nDW*4;
    }
  }
}

uint32_t* MDfragmentBM::GetTriggerEventPtr(unsigned int evId) {
  if ( evId >= _trigEvents.size() ) {
    stringstream ss;
    ss << "ERROR in MDfragmentBM::GetTriggerEventPtr() : ";
    ss << "Wrong Ebent Id: " << evId << ". Exceeds the total number of triggers." ;
    throw MDexception( ss.str() );
  }

  uint32_t *ptr = this->Get32bWordPtr( _trigEvents[evId] );
  return ptr;
}

void MDfragmentBM::Dump() {

}