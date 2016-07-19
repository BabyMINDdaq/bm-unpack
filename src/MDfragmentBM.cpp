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

void MDfragmentBM::Clean() {
  this->UnValidate();

  int nTr = _trigEvents.size();
  for (int i=0; i<nTr; ++i) {
    delete _trigEvents[i];
  }
  _trigEvents.resize(0);
}

void MDfragmentBM::Init() {
//     cout << " Calling MDfragmentBM::Init() " << endl;
  this->Clean();
  _size = 4;

  unsigned int * ptr = this->Get32bWordPtr(0);
  MDdataWordBM dw(ptr);
  if ( dw.IsValid() ) {
    // Check the reliability of the header and decode the header information.
    if (dw.GetDataType() != MDdataWordBM::SpillHeader ) { // The data doesn't start with a header
      throw MDexception("ERROR in MDfragmentBM::Init() : 1st word is not a spill header.");
    } else {
      _spillTag = dw.GetSpillTag();
      _boardId = dw.GetBoardId();
      bool done(false);
      ++ptr;
      while (!done) {
        dw.SetDataPtr(ptr);
        if (dw.GetDataType() == MDdataWordBM::TrigHeader) {
          MDpartEventBM *xPe = new MDpartEventBM(ptr);
          xPe->SetTriggerEvents(&_trigEvents);
          xPe->Init();
          unsigned int pe_size = xPe->GetSize();
          _size += pe_size;
          ptr += pe_size/4;
          //if (xPe->getNumDataWords() > 3) {
            _trigEvents.push_back( xPe );
          //} else {
            //delete xPe;
          //}
        } else if (dw.GetDataType() == MDdataWordBM::SpillTrailer1) {
          done = true;
          _size += 4;
        } else {
          cout << dw << endl;
          throw MDexception("ERROR in MDfragmentBM::Init() : Wrong data type.");
        }
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
      _size += 4;

      if ( dw.GetDataType() != MDdataWordBM::SpillTrailer1 ||
           dw.GetHeadTrailId()   !=  1 ||
           dw.GetBoardId()  != _boardId ) {
        throw MDexception("ERROR in MDfragmentBM::Init() : The spill trailer 1 is not consistent.");
      }
      _temperature = dw.GetTemperature();
      _humidity = dw.GetHumidity();

      dw.SetDataPtr(++ptr);
      _size += 4;
      cout << "Spill tag: " << _spillTag << "  Board Id: " << _boardId
           <<"  N triggers: " << _trigEvents.size() << "\n\n";
      if ( dw.GetDataType() != MDdataWordBM::SpillTrailer2 )
        throw MDexception("ERROR in MDfragmentBM::Init() : The spill trailer 2 is not consistent.");

    }
  }
}

MDpartEventBM* MDfragmentBM::GetTriggerEventPtr(unsigned int evId) {
  if ( evId >= _trigEvents.size() ) {
    stringstream ss;
    ss << "ERROR in MDfragmentBM::GetTriggerEventPtr() : ";
    ss << "Wrong Ebent Id: " << evId << ". Exceeds the total number of triggers." ;
    throw MDexception( ss.str() );
  }

  return _trigEvents[evId];
}

void MDfragmentBM::Dump() {

}