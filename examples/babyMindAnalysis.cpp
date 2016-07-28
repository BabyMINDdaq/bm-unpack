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

#include "MDfragmentBM.h"
#include "MDpartEventBM.h"
#include "MDargumentHandler.h"
#include "MDdataFile.h"

using namespace std;

char *dataBuff;
uint32_t* dataPtr;

int MaxParDist(vector<double> vec)
{
  int dist = 0;
  for(int i=1; i<vec.size();i++)
  {
    int diff = abs(vec[i]-vec[i-1]);
    if(diff>dist) dist=diff;
  }
  return dist;
}

int main( int argc, char **argv ) {
  string stringBuf;
  string filepath;
  string filename;

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

  string rootFilename = filename;
  rootFilename = rootFilename.substr(0, rootFilename.size()-4);
  rootFilename+=".root";

  TFile rfile(rootFilename.c_str(), "recreate");
  //TFile rfile("histos.root", "recreate");
  TH1I  h_lgah("h_lg_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_hgah("h_lh_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lth("h_le_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_tth("h_te_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lga("h_lg_ampl", "hit ampl.", 200, 0, 5000);
  TH1I  h_hga("h_hg_ampl", "hit ampl.", 200, 0, 5000);

  TH1D  h1("h1_nhit_ch", "nhit per channels", 100, 0, 100);
  TH1D  h2("h2_ampl", "hit ampl.", 200, 0, 1000);
  TH1D  h3("h3_spillID", "spill ID.", 20000, 0, 10000); 
  TH1D  h4("h4_time", "hit time", 20000, 0, 100000);
  TH1D  h5("h5_triggerTime", "trigger time", 2000, 0, 10000);
  TH1D  h6("h6_totalTime", "total time", 200000, 1000000, 1000000000);
  TH1D  h7("h7_spillTime", "spill time", 200000, 1000000, 1000000000);
  TH1D  h8("h8_triggerID", "trigger ID", 20000, 0, 100000); 

  TTree* t1 = new TTree("t1","Per spill tree");
  TTree* t2 = new TTree("t2","Per spill tree");

  TTree* t3 = new TTree("t3","Per trigger tree");

  long int nGoodHits=0;
  long int hitCand=0;

   vector<int> nHitsChannel;
   for(int i = 0; i<96; i++)
   {
    nHitsChannel.push_back(0);
   }

  vector<double> hitAmpl;
  vector<double> spillID;
  vector<double> hitTime;

  //vector<double> hitTimeTemp;
  //vector<vector<double> > hitTimePerSpill;
  vector<double> triggerTime;
  vector<double> totalTime;
  vector<double> spillTime;
  vector<double> triggerID;

  t1->Branch("nHitsChannel",&nHitsChannel);
  t1->Branch("hitAmpl",&hitAmpl);
  t1->Branch("spillID",&spillID);
  t1->Branch("hitTime",&hitTime);
  //t1.Branch("hitTimePerSpill",&hitTimePerSpill);
  t1->Branch("triggerTime",&triggerTime);
  t1->Branch("totalTime",&totalTime);
  t1->Branch("spillTime",&spillTime);
  t1->Branch("triggerID",&triggerID);


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


  vector<double> muonCandTrigger;
  int spillTagTrigger;
  int gTagTrigger;

  vector<double> barHitTrigger1;
  vector<double> barHitTrigger2;
  vector<double> barHitTrigger3;
  vector<double> barHitTrigger4;
  vector<double> barHitTrigger5;
  vector<double> barHitTrigger6;
  vector<vector<double> > febHitTrigger;
  vector<double> moduleTrigger;


  t3->Branch("muonCandTrigger",&muonCandTrigger);
  t3->Branch("spillTagTrigger",&spillTagTrigger);
  t3->Branch("gTagTrigger",&gTagTrigger);
  t3->Branch("febHitTrigger",&febHitTrigger);
  t3->Branch("barHitTrigger1",&barHitTrigger1);
  t3->Branch("barHitTrigger2",&barHitTrigger2);
  t3->Branch("barHitTrigger3",&barHitTrigger3);
  t3->Branch("barHitTrigger4",&barHitTrigger4);
  t3->Branch("barHitTrigger5",&barHitTrigger5);
  t3->Branch("barHitTrigger6",&barHitTrigger6);
  t3->Branch("moduleTrigger",&moduleTrigger);


  MDdateFile dfile(filename, filepath);
// Open the file and loop over event->.
  char *eventBuffer;
  if ( dfile.open() ) { // There is a valid files to unpack
    dfile.init();

    int xEv(0);
    do { // Loop over all spills
      eventBuffer =  dfile.GetNextEvent();


      cout<<"dfile.NSpills()="<<dfile.NSpills()<<endl;
      if(eventBuffer== NULL) break;


      try {
        MDfragmentBM   spill;
        spill.SetDataPtr(eventBuffer);

        //cout<<"spilltag="<<spill.GetSpillTag()<<endl;
        MDpartEventBM *event;
        int nTr = spill.GetNumOfTriggers();
        //cout<<"nTr="<<nTr<<endl;

        h3.Fill(spill.GetSpillTag());
        spillID.push_back(spill.GetSpillTag());
        //h7.Fill(spill.GetSpillTime());  
        //spillTime.push_back(spill.GetSpillTime());  

        spillTagInt =spill.GetSpillTag();
        spillTagTrigger =spill.GetSpillTag();

        int boardID = spill.GetBoardId();


        for (int i=0; i<nTr; ++i) {
          event = spill.GetTriggerEventPtr(i);
//           event->Dump();

          h5.Fill(event->GetTriggerTime());
          triggerTime.push_back(event->GetTriggerTime());
          h8.Fill(event->GetTriggerTag()); 
          triggerID.push_back(event->GetTriggerTag()); 

          triggerTimeVec.push_back(event->GetTriggerTime());
          triggerVec.push_back(i);

          gTagTrigger=i;
          int coincidentHits =0;
          int maxTime =0;
          int minTime =4000;





//Slowest part of code
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
                  deltaT.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
                    event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
                  

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
                    hit1.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos], 'h'));
                    hit2.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h'));
                    channel.push_back(channelVec[vecPos]+inChannelVec[vecPos]);
                    hitsTimeLeft.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'));
                    hitsTimeRight.push_back(event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
                    hitsTimeDiff.push_back((double)event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
                      (double)event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
                  //}
                  //}
                  //}
                  if(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
                    event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l') < 3)
                  {
                    coincidentHits++;
                    muonCandTrigger.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos],'l'));

                    int ich = channelVec[vecPos]+inChannelVec[vecPos];
                    if(boardID ==1 || boardID ==3)
                    {
                      if(ich < 16) barHitTrigger1.push_back(ich);
                      if((15<ich) && (ich<32)) barHitTrigger1.push_back(ich-16);
                      if((31<ich) && (ich<48)) barHitTrigger2.push_back(ich-32);
                      if((47<ich) && (ich<64)) barHitTrigger2.push_back(ich-48);
                      if((63<ich) && (ich<80)) barHitTrigger3.push_back(ich-64);
                      if((79<ich) && (ich<96)) barHitTrigger3.push_back(ich-80);
                    }
                    else if(boardID ==4 || boardID ==5)
                    {
                      if(ich < 16) barHitTrigger4.push_back(ich);
                      if((15<ich) && (ich<32)) barHitTrigger4.push_back(ich-16);
                      if((31<ich) && (ich<48)) barHitTrigger5.push_back(ich-32);
                      if((47<ich) && (ich<64)) barHitTrigger5.push_back(ich-48);
                      if((63<ich) && (ich<80)) barHitTrigger6.push_back(ich-64);
                      if((79<ich) && (ich<96)) barHitTrigger6.push_back(ich-80);
                    }

                    if(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l') > maxTime)
                    {
                      maxTime = event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l');
                    }
                    if(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l') < minTime)
                    {
                      minTime = event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l');
                    }
                  }



                }

              }
            }
          }

            if(coincidentHits == 3)
          {
            //Muon candidate;
            muonCandTrigg.push_back(i);
            muonDeltaT.push_back(maxTime-minTime);
          }

          if((coincidentHits < 2) ||(MaxParDist(muonCandTrigger)>2))
          {
            muonCandTrigger.clear();
            barHitTrigger1.clear();
            barHitTrigger2.clear();
            barHitTrigger3.clear();
            barHitTrigger4.clear();
            barHitTrigger5.clear();
            barHitTrigger6.clear();
          }
          int totHitsChannels =0;

          for (int ich=0; ich<BM_FEB_NCHANNELS; ++ich) {
            int nHits = event->GetNLeadingEdgeHits(ich);
            //hitCand+=event->GetNLeadingEdgeHits(ich);
            //hitCand+=event->GetNTrailingEdgeHits(ich);

            totHitsChannels+=nHits;
            if (nHits)
            {
              h1.Fill(ich, nHits);
              
              //tempHitsVec.push_back(nHits);
            }
                nHitsChannel[ich]+=nHits;
                nHitsChannelInt+=nHits;
                /*
                if(event->GetNLeadingEdgeHits(ich) <= event->GetNTrailingEdgeHits(ich))
                {
                  int numHits = event->GetNLeadingEdgeHits(ich);
                  for(int ih=0; ih<numHits; ih++)
                  {
                    if(event->GetHitTime(ih,ich, 'l')>0 && event->GetHitTime(ih,ich, 't')>0
                    && event->GetHitAmplitude(ich, 'h')>0 && event->GetHitAmplitude(ich, 'l')>0) nGoodHits++;
                  }
                }
                if(event->GetNLeadingEdgeHits(ich) >= event->GetNTrailingEdgeHits(ich))
                {
                  int numHits = event->GetNTrailingEdgeHits(ich);
                  for(int ih=0; ih<numHits; ih++)
                  {
                    if(event->GetHitTime(ih,ich, 'l')>0 && event->GetHitTime(ih,ich, 't')>0
                    && event->GetHitAmplitude(ich, 'h')>0 && event->GetHitAmplitude(ich, 'l')>0) nGoodHits++;
                  }
                }
                */

                if((event->GetNLeadingEdgeHits(ich) && event->GetNTrailingEdgeHits(ich)))
                {
                  //hitCand++;
                  hitCand+=event->GetNLeadingEdgeHits(ich) + event->GetNTrailingEdgeHits(ich);
                  if(event->GetHitAmplitude(ich, 'l')>0 && event->GetHitAmplitude(ich, 'h')>0)
                  {
                    for(int ih=0; ih<event->GetNLeadingEdgeHits(ich); ih++)
                    {
                      nGoodHits++;
                    }
                    for(int ih=0; ih<event->GetNTrailingEdgeHits(ich); ih++)
                    {
                      nGoodHits++;
                    }
                  }

                }

                
              double good_hits=0;
              for(int ih=0; ih<nHits; ih++)
              {

                h4.Fill(event->GetHitTime(ih,ich, 'l')); 
                hitTime.push_back(event->GetHitTime(ih,ich, 'l'));
                hitsTime.push_back(event->GetHitTime(ih,ich, 'l'));
                hitsAmp.push_back(event->GetHitAmplitude(ich, 'h') );

                //double sTime = spill.GetSpillTag();
                //sTime = sTime * 4096;
                //sTime = sTime * 1024;
                double tTime = event->GetHitTime(ih,ich, 'l')
                                 + event->GetTriggerTag() * 4000;
                                 //+ sTime;
                totalTime.push_back(tTime); 

                hitsChannel.push_back(ich);

                if(boardID ==1 || boardID ==3)
                {
                  if(event->GetHitTime(ih,ich, 'l') > 0)
                  {
                    if(ich < 16) barHit1.push_back(ich);
                    if((15<ich) && (ich<32)) barHit1.push_back(ich-16);
                    if((31<ich) && (ich<48)) barHit2.push_back(ich-32);
                    if((47<ich) && (ich<64)) barHit2.push_back(ich-48);
                    if((63<ich) && (ich<80)) barHit3.push_back(ich-64);
                    if((79<ich) && (ich<96)) barHit3.push_back(ich-80);
                  }
                }
                else if(boardID ==4 || boardID ==5)
                {
                  if(event->GetHitTime(ih,ich, 'l') > 0)
                  {
                    if(ich < 16) barHit4.push_back(ich);
                    if((15<ich) && (ich<32)) barHit4.push_back(ich-16);
                    if((31<ich) && (ich<48)) barHit5.push_back(ich-32);
                    if((47<ich) && (ich<64)) barHit5.push_back(ich-48);
                    if((63<ich) && (ich<80)) barHit6.push_back(ich-64);
                    if((79<ich) && (ich<96)) barHit6.push_back(ich-80);
                  }
                }


                if((event->GetHitTime(ih,ich, 'l') != 0)&& (event->GetHitAmplitude(ich, 'h') > 0))
                {
                  good_hits++;
                } 
                //hitTimeTemp.push_back(event->GetHitTime(ih,ich, 'l'));
                
                //hitTime *2.5ns
                //triggerTime * 10 micros
                //spillTime * 10 ms
                
                //cout<<tTime<<endl;
                h6.Fill(tTime); 
                
              }

            if(good_hits)
            {
              //hitsPerTrigger.push_back(good_hits);  
              goodHitsInt+=good_hits;
            }
              //cout<<good_hits<<endl;

              //hitsPerTrigger.push_back(tempHitsVec);
            tempHitsVec.clear();

            int q = event->GetHitAmplitude(ich, 'h');
            hitAmpl.push_back(q);
            h2.Fill(q);
          }//END for channels

          moduleTrigger.push_back(1);
          moduleTrigger.push_back(2);
          moduleTrigger.push_back(3);
          moduleTrigger.push_back(4);
          moduleTrigger.push_back(5);
          moduleTrigger.push_back(6);
          febHitTrigger.push_back(barHitTrigger1);
          febHitTrigger.push_back(barHitTrigger2);
          febHitTrigger.push_back(barHitTrigger3);
          febHitTrigger.push_back(barHitTrigger4);
          febHitTrigger.push_back(barHitTrigger5);
          febHitTrigger.push_back(barHitTrigger6);
          t3->Fill();
          muonCandTrigger.clear();
          moduleTrigger.clear();
          barHitTrigger1.clear();
          barHitTrigger2.clear();
          barHitTrigger3.clear();
          barHitTrigger4.clear();
          barHitTrigger5.clear();
          barHitTrigger6.clear();
          febHitTrigger.clear();

          hitsPerTrigger.push_back(totHitsChannels);


          for (int ich=0; ich<BM_FEB_NCHANNELS; ++ich) {
            int nHits = event->GetNLeadingEdgeHits(ich);
            if (nHits)
              h_lth.Fill(ich, nHits);

            nHits = event->GetNTrailingEdgeHits(ich);
            if (nHits)
              h_tth.Fill(ich, nHits);

            if (event->LGAmplitudeHitExists(ich)) {
              int q = event->GetHitAmplitude(ich, 'l');
              h_lga.Fill(q);
              h_lgah.Fill(ich);
            }

            if (event->HGAmplitudeHitExists(ich)) {
              int q = event->GetHitAmplitude(ich, 'h');
              h_hga.Fill(q);
              h_hgah.Fill(ich);
            }
          }
        }

        t1->Fill();  
        // Filled per spill.
        nHitsChannel.clear();
        hitAmpl.clear();
        spillID.clear();
        hitTime.clear();
  //vector<double> hitTimeTemp;
  //vector<vector<double> > hitTimePerSpill;
        triggerTime.clear();
        totalTime.clear();
        spillTime.clear();
        triggerID.clear();
        module.push_back(1);
        module.push_back(2);
        module.push_back(3);
        module.push_back(4);
        module.push_back(5);
        module.push_back(6);
        febHit.push_back(barHit1);
        febHit.push_back(barHit2);
        febHit.push_back(barHit3);
        febHit.push_back(barHit4);
        febHit.push_back(barHit5);
        febHit.push_back(barHit6);
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


      } catch (MDexception & lExc)  {
        std::cerr <<  lExc.GetDescription() << endl
                  << "Unpacking exception\n"
                  << "Spill skipped!\n\n";
      } catch(std::exception & lExc) {
        std::cerr << lExc.what() << std::endl
                  << "Standard exception\n"
                  << "Spill skipped!\n\n";
      } catch(...) {
        std::cerr << "Unknown exception occurred...\n"
                  << "Spill skipped!\n\n";
      }

      ++xEv;
//       } while (xEv < 5);
    } while ( eventBuffer );
  }

  dfile.close();

  h1.Write();
  h2.Write();
  h3.Write();
  h4.Write();
  h5.Write(); 
  h6.Write();
  h7.Write();
  h8.Write();

  //t1.Fill();  
  t1->Write();
  t2->Write();
  t3->Write();

  h_lgah.Write();
  h_hgah.Write();
  h_lth.Write();
  h_tth.Write();
  h_lga.Write();
  h_hga.Write();

  rfile.Close();
  dfile.close();

  cout<<"FigureOfMerit="<<(double)nGoodHits / (double)hitCand<<endl;
  //cout<<nGoodHits<<endl;
  //cout<<hitCand<<endl;
  
  delete dataBuff;
  return 0;
}



