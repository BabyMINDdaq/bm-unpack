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

#include <stdio.h>
#include <string.h>
#include <exception>
#include <fstream>

#include "TFile.h"
#include "TH1D.h"

#include "TTree.h"

#include <thread>
#include <mutex>
#include <signal.h>

#include "MDfragmentBM.h"
#include "MDpartEventBM.h"
#include "MDargumentHandler.h"
#include "MDdataFile.h"

using namespace std;

mutex mtx;           // mutex for critical section

char *dataBuff;
uint32_t* dataPtr;

  TH1I  h_lgah("h_lg_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_hgah("h_lh_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lth("h_le_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_tth("h_te_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lga("h_lg_ampl", "hit ampl.", 200, 0, 5000);
  TH1I  h_hga("h_hg_ampl", "hit ampl.", 200, 0, 5000);

 

  double nHitsChannelInt=0;
  int goodHitsInt=0;
  int spillTagInt;
  int numTrigger;
  vector<double> triggerTimeVec;
  vector<double> triggerVec;

  vector<double> noAmp;  
  vector<double> noTime;  
  vector<double> deltaTrigg;

  vector<double> tempHitsVec;
  vector<double> hitsPerTrigger;

  vector<double> hitsChannel;
  vector<double> hitsTime;
  vector<double> hitsTimeLeft;
  vector<double> hitsTimeRight;
  vector<double> hitsTimeDiff;
  vector<double> channel;
  vector<double> hit1;
  vector<double> hit2;  
  vector<double> hitsAmp;

  vector<double> deltaT;
  vector<double> muonCandTrigg;
  vector<double> muonDeltaT;

  vector<double> barHit1;
  vector<double> barHit2;
  vector<double> barHit3;
  vector<double> barHit4;
  vector<double> barHit5;
  vector<double> barHit6;
  vector<vector<double> > febHit;
  vector<double> module;

  vector<double> tester;



void HandleTrigger(MDpartEventBM* event,MDpartEventBM* nextEvent)
{
  vector<int> nFirstHitsVec;
  vector<int> nSecondHitsVec;
  vector<int> channelVec;
  vector<int> inChannelVec;

  for (int channel=0; channel<BM_FEB_NCHANNELS; channel+=32)
  {
    for(int inChannel=0; inChannel<16; inChannel++)
    {
      int nFirstHits = event->GetNLeadingEdgeHits(channel+inChannel);
      int nSecondHits = event->GetNLeadingEdgeHits(channel+inChannel+16);
      if (nFirstHits && nSecondHits)
      {
        nFirstHitsVec.push_back(nFirstHits);
        nSecondHitsVec.push_back(nSecondHits);
        channelVec.push_back(channel);
        inChannelVec.push_back(inChannel);
      }
    }
  }//End for channel
  for(int vecPos = 0; vecPos<nFirstHitsVec.size(); vecPos++)
  {
    for(int iFirsth=0; iFirsth<nFirstHitsVec[vecPos]; iFirsth++)
    {
      for(int iSecondh=0; iSecondh<nSecondHitsVec[vecPos]; iSecondh++)
      {
        if(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l') != 0 &&
          event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l') != 0 )
        {
          mtx.lock();
          deltaT.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
            event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
          mtx.unlock();
          //For Etam
          //if(channelVec[vecPos]+inChannelVec[vecPos] == 2)
          //{
            //if((event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos], 'h') >1400) &&
              //(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') >1400))
            //{
              //if((event->GetHitAmplitudeId(channelVec[vecPos]+inChannelVec[vecPos], 'h') == event->GetHitTimeId(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'))
              //  && (event->GetHitAmplitudeId(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') == event->GetHitTimeId(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'))
              //  && (event->GetHitAmplitudeTag(channelVec[vecPos]+inChannelVec[vecPos], 'h') == event->GetHitTimeTag(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'))
              //  && (event->GetHitAmplitudeTag(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') == event->GetHitTimeTag(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l')))
              //{
              //cout<<"AMPId="<<<<endl;
              //cout<<"AMPTag="<<<<endl;
              //cout<<"TIMEId="<<<<endl;
              //cout<<"TIMETag="<<<<endl;
          mtx.lock();
            hit1.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos], 'h'));
            hit2.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h'));
            channel.push_back(channelVec[vecPos]+inChannelVec[vecPos]);
            hitsTimeLeft.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'));
            hitsTimeRight.push_back(event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
            hitsTimeDiff.push_back((double)event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
              (double)event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
            mtx.unlock();
          //}
          //}
          //}
        }
      }
    }
  }


  for (int ich=0; ich<BM_FEB_NCHANNELS; ++ich) 
  {
    int nHits = event->GetNLeadingEdgeHits(ich);
    if (nHits)
    {
      mtx.lock();
      h_lth.Fill(ich, nHits);
      mtx.unlock();
    }

    nHits = event->GetNTrailingEdgeHits(ich);
    if (nHits)
    {
      mtx.lock();
      h_tth.Fill(ich, nHits);
      mtx.unlock();
    }

    if (event->LGAmplitudeHitExists(ich)) 
    {
      int q = event->GetHitAmplitude(ich, 'l');
      mtx.lock();
      h_lga.Fill(q);
      h_lgah.Fill(ich);
      mtx.unlock();
    }

    if (event->HGAmplitudeHitExists(ich)) 
    {
      int q = event->GetHitAmplitude(ich, 'h');
      mtx.lock();
      h_hga.Fill(q);
      h_hgah.Fill(ich);
      mtx.unlock();
    }
  }
}


void HandleSpill(MDfragmentBM* spill)
{
  int nTr = spill->GetNumOfTriggers();
  for (int i=0; i<nTr; ++i) 
  {
    MDpartEventBM *trigPtr = spill->GetTriggerEventPtr(i);

    MDpartEventBM *event = spill->GetTriggerEventPtr(i);;

    //event.SetDataPtr(trigPtr);
    HandleTrigger(event,event);
  }
}

int main( int argc, char **argv ) {
  string stringBuf;
  string filepath;
  string filename;

  TTree* t2 = new TTree("t2","Per spill tree");

  t2->Branch("tester",&tester);  

  t2->Branch("goodHitsInt",&goodHitsInt);  
  t2->Branch("nHitsChannelInt",&nHitsChannelInt);
  t2->Branch("hitsChannel",&hitsChannel);
  t2->Branch("hitsTime",&hitsTime);
  t2->Branch("hitsTimeLeft",&hitsTimeLeft);
  t2->Branch("hitsTimeRight",&hitsTimeRight);
  t2->Branch("hitsTimeDiff",&hitsTimeDiff);
  t2->Branch("channel",&channel);
  t2->Branch("hit1",&hit1);
  t2->Branch("hit2",&hit2);
  t2->Branch("hitsAmp",&hitsAmp);
  t2->Branch("spillTagInt",&spillTagInt);
  t2->Branch("numTrigger",&numTrigger);
  t2->Branch("triggerTimeVec",&triggerTimeVec);
  t2->Branch("triggerVec",&triggerVec);
  t2->Branch("noAmp",&noAmp);
  t2->Branch("noTime",&noTime);
  t2->Branch("deltaTrigg",&deltaTrigg);
  t2->Branch("hitsPerTrigger",&hitsPerTrigger);

  t2->Branch("muonCandTrigg",&muonCandTrigg);

  t2->Branch("muonDeltaT",&muonDeltaT);
  t2->Branch("deltaT",&deltaT);

  t2->Branch("febHit",&febHit);
  t2->Branch("barHit1",&barHit1);
  t2->Branch("barHit2",&barHit2);
  t2->Branch("barHit3",&barHit3);
  t2->Branch("barHit4",&barHit4);
  t2->Branch("barHit5",&barHit5);
  t2->Branch("barHit6",&barHit6);
  t2->Branch("module",&module);

  // The following shows how to use the MDargumentHandler class
  // to deal with the main arguments
  // Define the arguments
  MDargumentHandler argh("Example of unpacking application.");
  argh.AddArgument("help","print this message","h");
  argh.AddArgument("directory","path for the data file","d","<string>","." );
  argh.AddArgument("file","Name of a data file","f","<string>","mandatory");

  // Check the user arguments consistancy
  // All mandatory arguments should be provided and
  // There should be no extra arguments
  if ( argh.ProcessArguments(argc, argv) ) {argh.Usage(); return -1;}

  // Treat arguments, obtain values to be used later
  if ( argh.GetValue("help") ) {argh.Usage(); return 0;}
  if ( argh.GetValue("directory", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  filepath = stringBuf;

  if ( argh.GetValue("file", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  filename = stringBuf;

  TFile rfile("newMulti.root", "recreate");

  MDdateFile dfile(filename, filepath);
// Open the file and loop over events.
  char *eventBuffer;
  vector<MDfragmentBM*> spillVec;
  if ( dfile.open() ) { // There is a valid files to unpack
    dfile.init();

    int xEv(0);
    //do { // Loop over all spills
      eventBuffer =  dfile.GetNextEvent();
      
        while(eventBuffer != NULL)
        {
          try 
          {
            spillVec.push_back(new MDfragmentBM());
            spillVec.back()->SetDataPtr(eventBuffer);
            //eventBuffer =  dfile.GetNextEvent();

          } catch (MDexception & lExc)  {
          std::cerr <<  lExc.GetDescription() << endl
                    << "Unpacking exception\n"
                    << "Spill skipped!\n\n";
                    spillVec.erase(spillVec.begin()+spillVec.size()-1);
          } catch(std::exception & lExc) {
          std::cerr << lExc.what() << std::endl
                    << "Standard exception\n"
                    << "Spill skipped!\n\n";
                    spillVec.erase(spillVec.begin()+spillVec.size()-1);
          } catch(...) {
          std::cerr << "Unknown exception occurred...\n"
                    << "Spill skipped!\n\n";
                    spillVec.erase(spillVec.begin()+spillVec.size()-1);
      }
      
      eventBuffer =  dfile.GetNextEvent();


        }
        vector<thread> threads;

        for (unsigned int i = 0; i < spillVec.size(); ++i) 
        {
          mtx.lock();
          cout<<"started thread="<<i<<endl;
          mtx.unlock();
          threads.push_back(thread(HandleSpill, spillVec[i]));
          //HandleSpill(spillVec[i]);
        }

        for (unsigned int i = 0; i < threads.size(); ++i) 
        {
          threads[i].join();
          cout<<"spill "<<i<<" has joined"<<endl;
        }

        t2->Fill();  
        nHitsChannelInt=0;
        goodHitsInt=0;
        triggerTimeVec.clear();
        triggerVec.clear();
        hitsPerTrigger.clear();
        hitsChannel.clear();
        barHit1.clear();
        barHit2.clear();
        barHit3.clear();
        barHit4.clear();
        barHit5.clear();
        barHit6.clear();
        febHit.clear();
        hitsTime.clear();
        hitsTimeLeft.clear();
        hitsTimeRight.clear();
        hitsTimeDiff.clear();
        channel.clear();
        hit1.clear();
        hit2.clear();
        hitsAmp.clear();
        deltaT.clear();
        noTime.clear();
        noAmp.clear();
        muonCandTrigg.clear();
        muonDeltaT.clear();
        module.clear();



      ++xEv;
//       } while (xEv < 5);
    //} while ( eventBuffer );
  }

  dfile.close();

  h_lgah.Write();
  h_hgah.Write();
  h_lth.Write();
  h_tth.Write();
  h_lga.Write();
  h_hga.Write();

  t2->Write();

  rfile.Close();
  dfile.close();
  delete dataBuff;
  return 0;
}



