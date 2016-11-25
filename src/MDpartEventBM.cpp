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

#include "MDpartEventBM.h"

using namespace std;

MDpartEventBM::MDpartEventBM(void *d):MDdataContainer(d),
  _triggerTag(-1),_nDataWords(0), _trigEvents(0) {}

void MDpartEventBM::SetDataPtr( void *d, uint32_t aSize ) {
  MDdataContainer::SetDataPtr(d);
  this->Init();
}

void MDpartEventBM::Init() {
  //   cout << " Calling MDpartEventBM::Init() " << endl;
  this->UnValidate();

  for (int ich=0 ; ich < BM_FEB_NCHANNELS ; ich++) {
    _nLeadingEdgeHits[ich]=0;
    _nTrailingEdgeHits[ich]=0;
    _lgHit[ich]=false;
    _hgHit[ich]=false;
    _lgHitAmplitude[ich]=0;
    _hgHitAmplitude[ich]=0;
    _leadingEdgeHitTime[ich].clear();
    _trailingEdgeHitTime[ich].clear();
    _leadingEdgeHitId[ich].clear();
    _trailingEdgeHitId[ich].clear();
  }

  _nDataWords = 1;
  _triggerTag = -1;
  _size = 4;

  unsigned int * ptr = Get32bWordPtr(0);
  MDdataWordBM dw(ptr);
//   cout << dw << endl;
  if ( dw.IsValid() ) {
    // Check the reliability of the header and decode the header information.
    if (dw.GetDataType() != MDdataWordBM::TrigHeader ) { // The data doesn't start with a header
      throw MDexception("ERROR in MDpartEventBM::Init() : 1st word is not a trigger header");
    } else {
      _triggerTag = dw.GetTriggerTag();
      _triggerTagId = dw.GetTriggerTagShort();

      bool done(false);
      while (!done) {
        dw.SetDataPtr(++ptr);
        _size += 4;
//         cout << dw << endl;
        int dataType = dw.GetDataType();
        switch (dataType) {
          case MDdataWordBM::TimeMeas :
            if (dw.GetTagId() == _triggerTagId) {
              this->AddTimeHit(dw);
              ++_nDataWords;
            } else {
              if (_trigEvents) {
                int nTr = _trigEvents->size(), lastPending;
                lastPending = (nTr > 3) ? nTr-4 : 0;
                for (int i = nTr-1; i >= lastPending; --i) {
                  if (_trigEvents->at(i)->GetTriggerTagId() == dw.GetTagId()) {
                    _trigEvents->at(i)->AddTimeHit(dw);
                    break;
                  }
                }
              }
            }

            break;

          case MDdataWordBM::ChargeMeas :
            if (dw.GetTagId() == _triggerTagId) {
              this->AddAmplitudeHit(dw);
              ++_nDataWords;
            } else {
              if (_trigEvents) {
                int nTr = _trigEvents->size(), lastPending;
                lastPending = (nTr > 3) ? nTr-4 : 0;
                for (int i = nTr-1; i >= lastPending; --i) {
                  if (_trigEvents->at(i)->GetTriggerTagId() == dw.GetTagId()) {
                    _trigEvents->at(i)->AddAmplitudeHit(dw);
                    break;
                  }
                }
              }
            }

            break;

          case MDdataWordBM::TrigTrailer1 :
            done = true;
            ++_nDataWords;
            break;

          default :
            stringstream ss;
            ss << "ERROR in MDpartEventBM::Init() : Unexpected data word (id: "
               << dw.GetDataType() << ")";
            cout << dw << endl;
            throw MDexception(ss.str());
            break;
        }
      }

      if (dw.GetTriggerTag() != _triggerTag) {
        stringstream ss;
        ss << "ERROR in MDpartEventBM::Init() : The trigger trailer is not consistent \n(Trigger tag: "
           << dw.GetTriggerTag() << "!=" << _triggerTag << ")";
        throw MDexception(ss.str());
      }
      dw.SetDataPtr(++ptr);
      ++_nDataWords;
      _size +=4;
      _triggerTime = dw.GetTriggerTime();
      _hitCount = dw.GetHitCount();
    }
  }

  this->Validate();
}

void MDpartEventBM::AddTimeHit(MDdataWordBM &dw) {
  unsigned int xChan = dw.GetChannelId();
  if (dw.GetEdgeId()==0) {
    ++this->_nLeadingEdgeHits[xChan];
    this->_leadingEdgeHitTime[xChan].push_back(dw.GetHitTime());
    this->_leadingEdgeHitId[xChan].push_back(dw.GetHitId());
  } else {
    ++this->_nTrailingEdgeHits[xChan];
    this->_trailingEdgeHitTime[xChan].push_back(dw.GetHitTime());
    this->_trailingEdgeHitId[xChan].push_back(dw.GetHitId());
  }
}

void MDpartEventBM::AddAmplitudeHit(MDdataWordBM &dw) {
  unsigned int xChan = dw.GetChannelId();
  switch (dw.GetAmplitudeId()) {
    case MDdataWordBM::Amplitide_LG :
      _lgHitAmplitude[xChan] = dw.GetAmplitude();
      _lgHitAmplitudeId[xChan] = dw.GetHitId();
      _lgHit[xChan] = true;
      break;

    case MDdataWordBM::Amplitide_HG :
      _hgHitAmplitude[xChan] = dw.GetAmplitude();
      _hgHitAmplitudeId[xChan] = dw.GetHitId();
      _hgHit[xChan] = true;
      break;

    case MDdataWordBM::Baseline :
//       _amplitudeHits[xChan][2] = dw.GetAmplitude();
      break;

    default :
      stringstream ss;
      ss << "ERROR in MDpartEventBM::Init() : Unknown  Amplitide Id ( "
      << dw.GetAmplitudeId() << ")";
//       cout << ss.str() << endl;
      throw MDexception(ss.str());
  }
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
      if (ih<_nLeadingEdgeHits[ich]) { rv = _leadingEdgeHitTime[ich][ih]; }
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
      if (ih<_nTrailingEdgeHits[ich]) rv = _trailingEdgeHitTime[ich][ih];
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

unsigned int  MDpartEventBM::GetHitTimeId(unsigned int ih, unsigned int ich, char t) {
  int rv = 0xFFFFFFFF ;
  if ( ich > BM_FEB_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDpartEventBM::GetHitId() : ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }
  switch(t){
    case 'l':
    {
      if (ih<_nLeadingEdgeHits[ich]) { rv = _leadingEdgeHitId[ich][ih]; }
      else {
        stringstream ss;
        ss << "ERROR in MDpartEventBM::GetHitId() case l : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
    case 't':
    {
      if (ih<_nTrailingEdgeHits[ich]) rv = _trailingEdgeHitId[ich][ih];
      else {
        stringstream ss;
        ss << "ERROR in MDpartEventBM::GetHitId() case t : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
    default:
    {
      stringstream ss;
      ss << "ERROR in MDpartEventBM::GetHitId() : ";
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
      rv = _lgHitAmplitude[ich];
      break;

    case 'h':
      rv = _hgHitAmplitude[ich];
      break;

    default:
      stringstream ss;
      ss << "ERROR in MDpartEventBM::GetHitAmplitude(): ";
      ss << "Wrong argument: " << t;
      throw MDexception( ss.str() );

  }

  return rv;
}

unsigned int  MDpartEventBM::GetHitAmplitudeId(unsigned int ich, char t) {
  int rv = 0xFFFFFFFF ;
  if ( ich > BM_FEB_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDpartEventBM::GetHitAmplitudeId(): ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }

  switch(t) {
    case 'l':
      rv = _lgHitAmplitudeId[ich];
      break;

    case 'h':
      rv = _hgHitAmplitudeId[ich];
      break;

    default:
      stringstream ss;
      ss << "ERROR in MDpartEventBM::GetHitAmplitudeId(): ";
      ss << "Wrong argument: " << t;
      throw MDexception( ss.str() );

  }

  return rv;
}

void MDpartEventBM::Dump() {
  cout << *this;
}



ostream &operator<<(std::ostream &s, MDpartEventBM &pe) {

  s << " ++++++++++ BM Part Even ++++++++++ \n";
  s << " Tr. tag : " << pe.GetTriggerTag() << "(" << pe.GetTriggerTagId() << ")\n";
  s << " Tr. time : " << pe.GetTriggerTime() << "\n";
  s << " N hits: " << pe.GetHitCount()  << "\n";
  for (int ich=0 ; ich < BM_FEB_NCHANNELS ; ich++) {
    if (pe._lgHit[ich]) {
      s << " Ch: " << ich
        << "  Amplitude LG: " << pe.GetHitAmplitude(ich, 'l') << "  (hit Id: " << pe.GetHitAmplitudeId(ich, 'l') <<  ")\n";
    }

    if (pe._hgHit[ich]) {
      s << " Ch: " << ich
        << "  Amplitude HG: " << pe.GetHitAmplitude(ich, 'h') << "  (hit Id: " << pe.GetHitAmplitudeId(ich, 'h') <<  ")\n";
    }

    if (pe._nLeadingEdgeHits[ich]) {
      for (unsigned int ih=0; ih<pe._nLeadingEdgeHits[ich]; ++ih) {
        s << " Ch: " << ich
          << "  LTime: " << pe.GetHitTime(ih, ich, 'l') << "  (hit Id: " << pe.GetHitTimeId(ih, ich, 'l') << ")\n";
      }
    }

    if (pe._nTrailingEdgeHits[ich]) {
      for (unsigned int ih=0; ih<pe._nTrailingEdgeHits[ich]; ++ih) {
        s << " Ch: " << ich
          << "  TTime: " << pe.GetHitTime(ih, ich, 't') << "  (hit Id: " << pe.GetHitTimeId(ih, ich, 't') << ")\n";
      }
    }
  }

   s << " ++++++++++++++++++++++++++++++++++ \n\n";
  return s;
}


