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

#include "MDpartEventBM.h"
#include "MDdataWordBM.h"

using namespace std;

MDpartEventBM::MDpartEventBM(void *d):MDdataContainer(d),_triggerTag(0),_nDataWords(0) {
  this->Init();
}

void MDpartEventBM::SetDataPtr( void *d, uint32_t aSize ) {
  MDdataContainer::SetDataPtr(d);
  this->Init();
}

void MDpartEventBM::Init() {
  //   cout << " Calling MDpartEventBM::Init() " << endl;
  this->UnValidate();
  _nDataWords = 0;

  unsigned int * ptr = Get32bWordPtr(0);
  MDdataWordBM dw(ptr);

  for (int ich=0 ; ich < BM_FEB_NCHANNELS ; ich++) {
    _nLeadingEdgeHits[ich]=0;
    _nTrailingEdgeHits[ich]=0;
    _amplitudeHits[ich][0]=0;
    _amplitudeHits[ich][1]=0;
    _leadingEdgeHit[ich].clear();
    _trailingEdgeHit[ich].clear();
  }

//   cout << dw << endl;
  if ( dw.IsValid() ) {
    // Check the reliability of the header and decode the header information.
    if (dw.GetDataType() != MDdataWordBM::TrigHeader ){ // The data doesn't start with a header
      throw MDexception("ERROR in MDpartEventBM::Init() : 1st word is not a trigger header");
    } else {
      _triggerTag = dw.GetTriggerTag();

      bool done(false);
      while (!done) {
        dw.SetDataPtr(++ptr);
        ++_nDataWords;
//         cout << dw << endl;
        unsigned int xChan = dw.GetChannelId();
        int dataType = dw.GetDataType();
        switch (dataType) {
          case MDdataWordBM::TimeMeas :
            if (dw.GetEdgeId()==0) {
              ++_nLeadingEdgeHits[xChan];
              _leadingEdgeHit[xChan].push_back(dw.GetHitTime());
            } else {
              ++_nTrailingEdgeHits[xChan];
              _trailingEdgeHit[xChan].push_back(dw.GetHitTime());
            }
            break;

          case MDdataWordBM::ChargeMeas :

            switch (dw.GetAmplitudeId()) {
              case MDdataWordBM::Amplitide_LG :
                _amplitudeHits[xChan][0] = dw.GetAmplitude();
                break;

              case MDdataWordBM::Amplitide_HG :
                _amplitudeHits[xChan][1] = dw.GetAmplitude();
                break;

              case MDdataWordBM::Baseline :
//                 _amplitudeHits[xChan][2] = dw.GetAmplitude();
                break;

              default :
                stringstream ss;
                ss << "ERROR in MDpartEventBM::Init() : Unknown  Amplitide Id ( "
                   << dw.GetAmplitudeId() << ")";
                cout << ss.str() << endl;
//                throw MDexception(ss.str());
                break;
            }

            break;

          case MDdataWordBM::TrigTrailer1 :
            done = true;
            break;

          default :
            stringstream ss;
            ss << "ERROR in MDpartEventBM::Init() : Unexpected data word (id: "
               << dw.GetDataType() << ")";
//             cout << ss.str() << endl;
//             throw MDexception(ss.str());
            break;
        }
      }

      if (dw.GetTriggerTag() != _triggerTag) {
        throw MDexception("ERROR in MDpartEventBM::Init() : The trigger trailer is not consistent");
      }
      dw.SetDataPtr(++ptr);
      ++_nDataWords;
      _triggerTime = dw.GetTriggerTime();
      _hitCount = dw.GetHitCount();
      _size = _nDataWords*4;
    }
  }

  this->Validate();
}

unsigned int  MDpartEventBM::GetHitTime(unsigned int ih, unsigned int ich, char t) {
  int rv = 0xFFFFFFFF ;
  if ( ich > BM_FEB_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDpartEventBM::GetHitTime() : ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }
  switch(t){
    case 'l':
    {
      if (ih<_nLeadingEdgeHits[ich]) { rv = _leadingEdgeHit[ich][ih]; }
      else {
        stringstream ss;
        ss << "ERROR in MDpartEventBM::GetHitTime() case l : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
    case 't':
    {
      if (ih<_nTrailingEdgeHits[ich]) rv = _trailingEdgeHit[ich][ih];
      else {
        stringstream ss;
        ss << "ERROR in MDpartEventBM::GetHitTime() case t : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
    default:
    {
      stringstream ss;
      ss << "ERROR in MDpartEventBM::GetHitTime() : ";
      ss << "Wrong argument: t = " << t;
      throw MDexception( ss.str() );
    }
  }

  return rv;
}


unsigned int  MDpartEventBM::GetHitAmplitude(unsigned int ich, char t) {
  int rv = 0xFFFFFFFF ;
  if ( ich > BM_FEB_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDpartEventBM::GetHitAmplitude(): ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }

  switch(t) {
    case 'l':
      rv = _amplitudeHits[ich][0];
      break;

    case 'h':
      rv = _amplitudeHits[ich][1];
      break;

    default:
      stringstream ss;
      ss << "ERROR in MDpartEventBM::GetHitAmplitude(): ";
      ss << "Wrong argument: " << t;
      throw MDexception( ss.str() );

  }

  return rv;
}

