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
#include "TH2I.h"
#include "TH2D.h"
#include "TTree.h"
#include "TF1.h"

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
  string stringBuf2;
  string filepath;
  string filename;
  string filename2;

  // The following shows how to use the MDargumentHandler class
  // to deal with the main arguments
  // Define the arguments
  MDargumentHandler argh("Example of unpacking application.");
  argh.AddArgument("help","print this message","h");
  argh.AddArgument("directory","path for the data file","d","<string>","." );
  argh.AddArgument("file","Name of a data file","f","<string>","mandatory");
  argh.AddArgument("file2","Name of a data file","g","<string>","mandatory");

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

  if ( argh.GetValue("file2", stringBuf2) != MDARGUMENT_STATUS_OK ) return -1;
  filename2 = stringBuf2;

  string rootFilename = filename;
  rootFilename = rootFilename.substr(0, rootFilename.size()-4);
  rootFilename+=".root";

  //TFile rfile(rootFilename.c_str(), "recreate");
  TFile rfile("analysis2.root", "recreate");
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

  TH2D  beamHistoLG1("beamHistoLG1", "beamHistoLG1", 16, 0, 16,16,0,16);
  TH2D  beamHistoHG1("beamHistoHG1", "beamHistoHG1", 16, 0, 16,16,0,16);
  TH2D  beamHistoLG2("beamHistoLG2", "beamHistoLG2", 16, 0, 16,16,0,16);
  TH2D  beamHistoHG2("beamHistoHG2", "beamHistoHG2", 16, 0, 16,16,0,16);
  TH2D  beamHistoLG3("beamHistoLG3", "beamHistoLG3", 16, 0, 16,16,0,16);
  TH2D  beamHistoHG3("beamHistoHG3", "beamHistoHG3", 16, 0, 16,16,0,16);

  TH2D  beamHistoLGMean1("beamHistoLGMean1", "beamHistoLGMean1", 16, 0, 16,16,0,16);
  TH2D  beamHistoLGSigma1("beamHistoLGSigma1", "beamHistoLGSigma1", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGMean1("beamHistoHGMean1", "beamHistoHGMean1", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGSigma1("beamHistoHGSigma1", "beamHistoHGSigma1", 16, 0, 16,16,0,16);
  TH2D  beamHistoLEMean1("beamHistoLEMean1", "beamHistoLEMean1", 16, 0, 16,16,0,16);
  TH2D  beamHistoLESigma1("beamHistoLESigma1", "beamHistoLESigma1", 16, 0, 16,16,0,16);
  TH2D  beamHistoTEMean1("beamHistoTEMean1", "beamHistoTEMean1", 16, 0, 16,16,0,16);
  TH2D  beamHistoTESigma1("beamHistoTESigma1", "beamHistoTESigma1", 16, 0, 16,16,0,16);
  TH2D  timeDiffMean1("timeDiffMean1", "timeDiffMean1", 16, 0, 16,16,0,16);
  TH2D  timeDiffSigma1("timeDiffSigma1", "timeDiffSigma1", 16, 0, 16,16,0,16);

  TH2D  beamHistoLGMean2("beamHistoLGMean2", "beamHistoLGMean2", 16, 0, 16,16,0,16);
  TH2D  beamHistoLGSigma2("beamHistoLGSigma2", "beamHistoLGSigma2", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGMean2("beamHistoHGMean2", "beamHistoHGMean2", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGSigma2("beamHistoHGSigma2", "beamHistoHGSigma2", 16, 0, 16,16,0,16);
  TH2D  beamHistoLEMean2("beamHistoLEMean2", "beamHistoLEMean2", 16, 0, 16,16,0,16);
  TH2D  beamHistoLESigma2("beamHistoLESigma2", "beamHistoLESigma2", 16, 0, 16,16,0,16);
  TH2D  beamHistoTEMean2("beamHistoTEMean2", "beamHistoTEMean2", 16, 0, 16,16,0,16);
  TH2D  beamHistoTESigma2("beamHistoTESigma2", "beamHistoTESigma2", 16, 0, 16,16,0,16);
  TH2D  timeDiffMean2("timeDiffMean2", "timeDiffMean2", 16, 0, 16,16,0,16);
  TH2D  timeDiffSigma2("timeDiffSigma2", "timeDiffSigma2", 16, 0, 16,16,0,16);

  TH2D  beamHistoLGMean3("beamHistoLGMean3", "beamHistoLGMean3", 16, 0, 16,16,0,16);
  TH2D  beamHistoLGSigma3("beamHistoLGSigma3", "beamHistoLGSigma3", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGMean3("beamHistoHGMean3", "beamHistoHGMean3", 16, 0, 16,16,0,16);
  TH2D  beamHistoHGSigma3("beamHistoHGSigma3", "beamHistoHGSigma3", 16, 0, 16,16,0,16);
  TH2D  beamHistoLEMean3("beamHistoLEMean3", "beamHistoLEMean3", 16, 0, 16,16,0,16);
  TH2D  beamHistoLESigma3("beamHistoLESigma3", "beamHistoLESigma3", 16, 0, 16,16,0,16);
  TH2D  beamHistoTEMean3("beamHistoTEMean3", "beamHistoTEMean3", 16, 0, 16,16,0,16);
  TH2D  beamHistoTESigma3("beamHistoTESigma3", "beamHistoTESigma3", 16, 0, 16,16,0,16);
  TH2D  timeDiffMean3("timeDiffMean3", "timeDiffMean3", 16, 0, 16,16,0,16);
  TH2D  timeDiffSigma3("timeDiffSigma3", "timeDiffSigma3", 16, 0, 16,16,0,16);


  // Create temp histograms to calculate Amp mean, sigma and time mean, sigma for each channel.
  vector<TH1I*> lgAmp1, lgAmp2;
  vector<TH1I*> hgAmp1, hgAmp2;
  vector<TH1I*> le1, le2;
  vector<TH1I*> te1, te2;

  vector<TH1D*> timeDiff1, timeDiff2;
  for(int i=0; i<96; i++)
  {
    stringstream strs;
    strs << i;
    string nameLGAmpStr1 = "lgAmp1"+strs.str();
    char* nameLGAmp1 = (char*) nameLGAmpStr1.c_str();
    string nameLGAmpStr2 = "lgAmp2"+strs.str();
    char* nameLGAmp2 = (char*) nameLGAmpStr2.c_str();

    string nameHGAmpStr1 = "hgAmp1"+strs.str();
    char* nameHGAmp1 = (char*) nameHGAmpStr1.c_str();
    string nameHGAmpStr2 = "hgAmp2"+strs.str();
    char* nameHGAmp2 = (char*) nameHGAmpStr2.c_str();

    string nameLEStr1 = "LE1"+strs.str();
    char* nameLE1 = (char*) nameLEStr1.c_str();
    string nameLEStr2 = "LE2"+strs.str();
    char* nameLE2 = (char*) nameLEStr2.c_str();

    string nameTEStr1 = "TE1"+strs.str();
    char* nameTE1 = (char*) nameTEStr1.c_str();
    string nameTEStr2 = "TE2"+strs.str();
    char* nameTE2 = (char*) nameTEStr2.c_str();

    string nametimeDiffStr1 = "timeDiff1"+strs.str();
    char* nametimeDiff1 = (char*) nametimeDiffStr1.c_str();
    string nametimeDiffStr2 = "timeDiff2"+strs.str();
    char* nametimeDiff2 = (char*) nametimeDiffStr2.c_str();

    lgAmp1.push_back(new TH1I(nameLGAmp1, nameLGAmp1, 200, 0, 5000));
    lgAmp2.push_back(new TH1I(nameLGAmp2, nameLGAmp2, 200, 0, 5000));
    hgAmp1.push_back(new TH1I(nameHGAmp1, nameHGAmp1, 200, 0, 5000));
    hgAmp2.push_back(new TH1I(nameHGAmp2, nameHGAmp2, 200, 0, 5000));

    le1.push_back(new TH1I(nameLE1, nameLE1, 200, 0, 5000));
    le2.push_back(new TH1I(nameLE2, nameLE2, 200, 0, 5000));
    te1.push_back(new TH1I(nameTE1, nameTE1, 200, 0, 5000));
    te2.push_back(new TH1I(nameTE2, nameTE2, 200, 0, 5000));

    timeDiff1.push_back(new TH1D(nametimeDiff1, nametimeDiff1, 16, -4, 4));
    timeDiff2.push_back(new TH1D(nametimeDiff2, nametimeDiff2, 16, -4, 4));
  }




  TTree* t1 = new TTree("t1","Per spill tree");
  TTree* t2 = new TTree("t2","Per spill tree");

  TTree* t3 = new TTree("t3","Per trigger tree");

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

  vector<double> hitsTimeLeft2;
  vector<double> hitsTimeRight2;
  vector<double> hitsTimeDiff2;
  vector<double> channel2;
  vector<double> hit21;
  vector<double> hit22;

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
  t2->Branch("hitsTimeLeft2",&hitsTimeLeft2);
  t2->Branch("hitsTimeRight2",&hitsTimeRight2);
  t2->Branch("hitsTimeDiff2",&hitsTimeDiff2);
  t2->Branch("channel2",&channel2);
  t2->Branch("hit21",&hit21);
  t2->Branch("hit22",&hit22);
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
  MDdateFile dfile2(filename2, filepath);

//MDdateFile dfile("../dataE/feb1_std_10gevmuons_extscint16_nogarbage_refdata4.daq",filepath);
//MDdateFile dfile2("../dataE/feb3_std_10gevmuons_extscint16_nogarbage_refdata4.daq",filepath);
 

// Open the file and loop over event->.
  char *eventBuffer;

  char *eventBuffer2;

  if ( dfile.open() && dfile2.open()) { // There is a valid files to unpack
    //if ( dfile.open()) {
    dfile.init();
    dfile2.init();
    int counter = 0;
    int xEv(0);
    do { // Loop over all spills
      eventBuffer =  dfile.GetNextEvent();
      eventBuffer2 =  dfile2.GetNextEvent();

      if(eventBuffer== NULL || eventBuffer2== NULL) break;
      //if(eventBuffer== NULL) break;  
      //cout<<"spill="<<counter<<endl;
      counter++;

      if(counter==10) break;

      try {
        MDfragmentBM   spill;
        MDfragmentBM   spill2;
        spill.SetDataPtr(eventBuffer);
        spill2.SetDataPtr(eventBuffer2);
        //cout<<"spilltag="<<spill.GetSpillTag()<<endl;
        MDpartEventBM *event;
        MDpartEventBM  *event2;
        int nTr = spill.GetNumOfTriggers();
        int nTr2 = spill2.GetNumOfTriggers();

        if(nTr!=nTr2)
        {
          cout<<"Different number of triggers"<<endl;
          continue;
        } 

        //cout<<"nTr="<<nTr<<endl;

        h3.Fill(spill.GetSpillTag());
        spillID.push_back(spill.GetSpillTag());
        //h7.Fill(spill.GetSpillTime());  
        //spillTime.push_back(spill.GetSpillTime());  

        spillTagInt =spill.GetSpillTag();
        spillTagTrigger =spill.GetSpillTag();

        int boardID = spill.GetBoardId();

        //if(nTr2<nTr) nTr=nTr2;


        for (int i=0; i<nTr; ++i) {


          //cout<<"trigger="<<i<<endl;
          event = spill.GetTriggerEventPtr(i);
          event2 = spill2.GetTriggerEventPtr(i);

          //beamHisto.Fill((Double_t)0,(Double_t)0,(Double_t)1);
          //beamHisto.Fill((Double_t)15,(Double_t)15,(Double_t)1);
          //beamHisto.Fill((Double_t)7,(Double_t)7,(Double_t)1);

          // Beam profile.
          for (int channel1=0; channel1<80; channel1++)
          {
            if(event->LGAmplitudeHitExists(channel1)) lgAmp1[channel1]->Fill(event->GetHitAmplitude(channel1, 'l'));
            if(event2->LGAmplitudeHitExists(channel1)) lgAmp2[channel1]->Fill(event2->GetHitAmplitude(channel1, 'l'));
            if(event->HGAmplitudeHitExists(channel1)) hgAmp1[channel1]->Fill(event->GetHitAmplitude(channel1, 'h'));
            if(event2->HGAmplitudeHitExists(channel1)) hgAmp2[channel1]->Fill(event2->GetHitAmplitude(channel1, 'h'));

            if(event->LGAmplitudeHitExists(channel1) && event->LGAmplitudeHitExists(channel1+16))
            {
              for (int channel2=0; channel2<16; channel2++)
              {
                if(event2->LGAmplitudeHitExists(channel2) && event2->LGAmplitudeHitExists(channel2+16)) beamHistoLG1.Fill(channel1, channel2, 1);
              }
              for (int channel2=32; channel2<48; channel2++)
              {
                if(event2->LGAmplitudeHitExists(channel2) && event2->LGAmplitudeHitExists(channel2+16)) beamHistoLG2.Fill(channel1-32, channel2-32, 1);
              }
              for (int channel2=64; channel2<80; channel2++)
              {
                if(event2->LGAmplitudeHitExists(channel2) && event2->LGAmplitudeHitExists(channel2+16)) beamHistoLG3.Fill(channel1-64, channel2-64, 1);
              }
            }
            if(event->HGAmplitudeHitExists(channel1) && event->HGAmplitudeHitExists(channel1+16))
            {
              for (int channel2=0; channel2<16; channel2++)
              {
                if(event2->HGAmplitudeHitExists(channel2) && event2->HGAmplitudeHitExists(channel2+16)) beamHistoHG1.Fill(channel1, channel2, 1);
              }
              for (int channel2=32; channel2<48; channel2++)
              {
                if(event2->HGAmplitudeHitExists(channel2) && event2->HGAmplitudeHitExists(channel2+16)) beamHistoHG2.Fill(channel1-32, channel2-32, 1);
              }
              for (int channel2=64; channel2<80; channel2++)
              {
                if(event2->HGAmplitudeHitExists(channel2) && event2->HGAmplitudeHitExists(channel2+16)) beamHistoHG3.Fill(channel1-64, channel2-64, 1);
              }
          }
        }

          for (int channel=0; channel<BM_FEB_NCHANNELS; channel++)
          {
            if(event->LGAmplitudeHitExists(channel)) lgAmp1[channel]->Fill(event->GetHitAmplitude(channel, 'l'));
            if(event2->LGAmplitudeHitExists(channel)) lgAmp2[channel]->Fill(event2->GetHitAmplitude(channel, 'l'));
            if(event->HGAmplitudeHitExists(channel)) hgAmp1[channel]->Fill(event->GetHitAmplitude(channel, 'h'));
            if(event2->HGAmplitudeHitExists(channel)) hgAmp2[channel]->Fill(event2->GetHitAmplitude(channel, 'h'));
          
            le1[channel]->Fill(event->GetNLeadingEdgeHits(channel));
            le2[channel]->Fill(event2->GetNLeadingEdgeHits(channel));
            te1[channel]->Fill(event->GetNTrailingEdgeHits(channel));
            te2[channel]->Fill(event2->GetNTrailingEdgeHits(channel));
          }

          
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

          vector<int> nFirstHitsVec2;
          vector<int> nSecondHitsVec2;
          vector<int> channelVec2;
          vector<int> inChannelVec2;

          for (int channel=0; channel<BM_FEB_NCHANNELS; channel+=32)
          {
            for(int inChannel=0; inChannel<16; inChannel++)
            {
              int nFirstHits = event->GetNLeadingEdgeHits(channel+inChannel);
              int nSecondHits = event->GetNLeadingEdgeHits(channel+inChannel+16);

              int nFirstHits2 = event2->GetNLeadingEdgeHits(channel+inChannel);
              int nSecondHits2 = event2->GetNLeadingEdgeHits(channel+inChannel+16);

              if (nFirstHits && nSecondHits && nFirstHits2 && nSecondHits2)
              {
                nFirstHitsVec.push_back(nFirstHits);
                nSecondHitsVec.push_back(nSecondHits);
                channelVec.push_back(channel);
                inChannelVec.push_back(inChannel);

                nFirstHitsVec2.push_back(nFirstHits2);
                nSecondHitsVec2.push_back(nSecondHits2);
                channelVec2.push_back(channel);
                inChannelVec2.push_back(inChannel);
              }
            }
          }//End for channel

          //for(int channelSum=0;channelSum<BM_FEB_NCHANNELS;channelSum++)
          //{
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
                  
                  int channelSum = channelVec[vecPos]+inChannelVec[vecPos];
                  timeDiff1[channelSum]->Fill((double)event->GetHitTime(iFirsth,channelSum, 'l')-
                    (double)event->GetHitTime(iSecondh,channelSum+16, 'l'));
                  
                  //For Etam
                  if(channelSum == 8)
                  {
                    //if((event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos], 'h') >1400) &&
                      //(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') >1400))
                    //{
                      
                      //if((event->GetHitAmplitudeId(channelVec[vecPos]+inChannelVec[vecPos], 'h') == event->GetHitTimeId(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'))
                      //  && (event->GetHitAmplitudeId(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') == event->GetHitTimeId(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'))
                      //  && (event->GetHitAmplitudeTag(channelVec[vecPos]+inChannelVec[vecPos], 'h') == event->GetHitTimeTag(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'))
                      //  && (event->GetHitAmplitudeTag(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h') == event->GetHitTimeTag(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l')))
                      //{
                    hit1.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos], 'h'));
                    hit2.push_back(event->GetHitAmplitude(channelVec[vecPos]+inChannelVec[vecPos]+16, 'h'));
                    channel.push_back(channelVec[vecPos]+inChannelVec[vecPos]);
                    hitsTimeLeft.push_back(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l'));
                    hitsTimeRight.push_back(event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
                    hitsTimeDiff.push_back((double)event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l')-
                      (double)event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l'));
                  //}
                  }
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
          for(int vecPos = 0; vecPos<nFirstHitsVec2.size(); vecPos++)
          {
            for(int iFirsth=0; iFirsth<nFirstHitsVec2[vecPos]; iFirsth++)
            {
              for(int iSecondh=0; iSecondh<nSecondHitsVec2[vecPos]; iSecondh++)
              {
                if(event2->GetHitTime(iFirsth,channelVec2[vecPos]+inChannelVec2[vecPos], 'l') != 0 &&
                  event2->GetHitTime(iSecondh,channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'l') != 0 )
                {   
                  int channelSum = channelVec2[vecPos]+inChannelVec2[vecPos];
                  timeDiff2[channelSum]->Fill((double)event2->GetHitTime(iFirsth,channelSum, 'l')-
                    (double)event2->GetHitTime(iSecondh,channelSum+16, 'l'));

                   if(channelSum == 8)
                  {
                      //if((event2->GetHitAmplitudeId(channelVec2[vecPos]+inChannelVec2[vecPos], 'h') == event2->GetHitTimeId(iFirsth,channelVec2[vecPos]+inChannelVec2[vecPos], 'l'))
                      //  && (event2->GetHitAmplitudeId(channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'h') == event2->GetHitTimeId(iSecondh,channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'l'))
                      //  && (event2->GetHitAmplitudeTag(channelVec2[vecPos]+inChannelVec2[vecPos], 'h') == event2->GetHitTimeTag(iFirsth,channelVec2[vecPos]+inChannelVec2[vecPos], 'l'))
                      //  && (event2->GetHitAmplitudeTag(channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'h') == event2->GetHitTimeTag(iSecondh,channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'l')))
                      //{
                    hit21.push_back(event2->GetHitAmplitude(channelVec2[vecPos]+inChannelVec2[vecPos], 'h'));
                    hit22.push_back(event2->GetHitAmplitude(channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'h'));
                    channel2.push_back(channelVec2[vecPos]+inChannelVec2[vecPos]);
                    hitsTimeLeft2.push_back(event2->GetHitTime(iFirsth,channelVec2[vecPos]+inChannelVec2[vecPos], 'l'));
                    hitsTimeRight2.push_back(event2->GetHitTime(iSecondh,channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'l'));
                    hitsTimeDiff2.push_back((double)event2->GetHitTime(iFirsth,channelVec2[vecPos]+inChannelVec2[vecPos], 'l')-
                      (double)event2->GetHitTime(iSecondh,channelVec2[vecPos]+inChannelVec2[vecPos]+16, 'l'));
                  //}
                  }
                  //}
                      
                }

              }
            }
          }
          //}

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
            totHitsChannels+=nHits;
            if (nHits)
            {
              h1.Fill(ich, nHits);
              
              //tempHitsVec.push_back(nHits);
            }
                nHitsChannel[ich]+=nHits;
                nHitsChannelInt+=nHits;
                
                
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
        hitsTimeLeft2.clear();
        hitsTimeRight2.clear();
        hitsTimeDiff2.clear();
        channel2.clear();
        hit21.clear();
        hit22.clear();
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
  beamHistoLG1.Write();
  beamHistoHG1.Write();
  beamHistoLG2.Write();
  beamHistoHG2.Write();
  beamHistoLG3.Write();
  beamHistoHG3.Write();
/*
   rC->Fit("gaus","Q");
    TF1* gaus = rC->GetFunction("gaus");
    mean = gaus->GetParameter(1);
    rms = gaus->GetParameter(2);

    //double mean = rC->GetMean();
    //double entries = rC->GetEntries();
    //double rms = rC->GetRMS();
*/

  for (int channel1=0; channel1<BM_FEB_NCHANNELS; channel1++)
  {
    if((16<channel1&&channel1<32) || (48<channel1&&channel1<64) || (80<channel1&&channel1<96)) continue;

    for (int channel2=0; channel2<BM_FEB_NCHANNELS; channel2++)
    {
      if((16<channel2&&channel2<32) || (48<channel2&&channel2<64) || (80<channel2&&channel2<96)) continue;


      lgAmp1[channel1]->Fit("gaus","Q");
      TF1* gauslgAmp1 = lgAmp1[channel1]->GetFunction("gaus");
      lgAmp2[channel2]->Fit("gaus","Q");
      TF1* gauslgAmp2 = lgAmp2[channel2]->GetFunction("gaus");

      hgAmp1[channel1]->Fit("gaus","Q");
      TF1* gaushgAmp1 = hgAmp1[channel1]->GetFunction("gaus");
      hgAmp2[channel2]->Fit("gaus","Q");
      TF1* gaushgAmp2 = hgAmp2[channel2]->GetFunction("gaus");

      le1[channel1]->Fit("gaus","Q");
      TF1* gausle1 = le1[channel1]->GetFunction("gaus");
      le2[channel2]->Fit("gaus","Q");
      TF1* gausle2 = le2[channel2]->GetFunction("gaus");

      te1[channel1]->Fit("gaus","Q");
      TF1* gauste1 = te1[channel1]->GetFunction("gaus");
      te2[channel2]->Fit("gaus","Q");
      TF1* gauste2 = te2[channel2]->GetFunction("gaus");

      double gaustimeDiff1Mean =10;
      double gaustimeDiff2Mean =10;
      double gaustimeDiff1Sigma =10;
      double gaustimeDiff2Sigma =10;

      if(timeDiff1[channel1]->GetEntries()>10 && timeDiff2[channel2]->GetEntries()>10)
      {
      timeDiff1[channel1]->Fit("gaus","Q");
      TF1* gaustimeDiff1 = timeDiff1[channel1]->GetFunction("gaus");
      timeDiff2[channel2]->Fit("gaus","Q");
      TF1* gaustimeDiff2 = timeDiff2[channel2]->GetFunction("gaus");

      gaustimeDiff1Mean=gaustimeDiff1->GetParameter(1);
      gaustimeDiff2Mean=gaustimeDiff2->GetParameter(1);
      gaustimeDiff1Sigma=gaustimeDiff1->GetParameter(1);
      gaustimeDiff2Sigma=gaustimeDiff2->GetParameter(2);
      }



      if(channel1<16 && channel2<16)
      {
      beamHistoLGMean1.Fill(channel1, channel2, gauslgAmp1->GetParameter(1)+gauslgAmp2->GetParameter(1));
      beamHistoLGSigma1.Fill(channel1, channel2, gauslgAmp1->GetParameter(2)+gauslgAmp2->GetParameter(2));
      //beamHistoLGMean.Fill(channel1, channel2, lgAmp1[channel1]->GetMean()+lgAmp2[channel2]->GetMean());
      //beamHistoLGSigma.Fill(channel1, channel2, lgAmp1[channel1]->GetRMS()+lgAmp2[channel2]->GetRMS());
      beamHistoHGMean1.Fill(channel1, channel2, gaushgAmp1->GetParameter(1)+gaushgAmp2->GetParameter(1));
      beamHistoHGSigma1.Fill(channel1, channel2, gaushgAmp1->GetParameter(2)+gaushgAmp2->GetParameter(2));
    
      beamHistoLEMean1.Fill(channel1, channel2, gausle1->GetParameter(1)+gausle2->GetParameter(1));
      beamHistoLESigma1.Fill(channel1, channel2, gausle1->GetParameter(2)+gausle2->GetParameter(2));

      beamHistoTEMean1.Fill(channel1, channel2, gauste1->GetParameter(1)+gauste2->GetParameter(1));
      beamHistoTESigma1.Fill(channel1, channel2, gauste1->GetParameter(2)+gauste2->GetParameter(2));

      timeDiffMean1.Fill(channel1, channel2, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma1.Fill(channel1, channel2, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
      else if(32<channel1 && channel1<48 && 32<channel2 && channel2<48)
      {
      beamHistoLGMean2.Fill(channel1-32, channel2-32, gauslgAmp1->GetParameter(1)+gauslgAmp2->GetParameter(1));
      beamHistoLGSigma2.Fill(channel1-32, channel2-32, gauslgAmp1->GetParameter(2)+gauslgAmp2->GetParameter(2));
      
      beamHistoHGMean2.Fill(channel1-32, channel2-32, gaushgAmp1->GetParameter(1)+gaushgAmp2->GetParameter(1));
      beamHistoHGSigma2.Fill(channel1-32, channel2-32, gaushgAmp1->GetParameter(2)+gaushgAmp2->GetParameter(2));
    
      beamHistoLEMean2.Fill(channel1-32, channel2-32, gausle1->GetParameter(1)+gausle2->GetParameter(1));
      beamHistoLESigma2.Fill(channel1-32, channel2-32, gausle1->GetParameter(2)+gausle2->GetParameter(2));

      beamHistoTEMean2.Fill(channel1-32, channel2-32, gauste1->GetParameter(1)+gauste2->GetParameter(1));
      beamHistoTESigma2.Fill(channel1-32, channel2-32, gauste1->GetParameter(2)+gauste2->GetParameter(2));

      timeDiffMean2.Fill(channel1-32, channel2-32, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma2.Fill(channel1-32, channel2-32, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
      else if(64<channel1 && channel1<80 && 64<channel2 && channel2<80)
      {
      beamHistoLGMean3.Fill(channel1-64, channel2-64, gauslgAmp1->GetParameter(1)+gauslgAmp2->GetParameter(1));
      beamHistoLGSigma3.Fill(channel1-64, channel2-64, gauslgAmp1->GetParameter(2)+gauslgAmp2->GetParameter(2));
      beamHistoHGMean3.Fill(channel1-64, channel2-64, gaushgAmp1->GetParameter(1)+gaushgAmp2->GetParameter(1));
      beamHistoHGSigma3.Fill(channel1-64, channel2-64, gaushgAmp1->GetParameter(2)+gaushgAmp2->GetParameter(2));
    
      beamHistoLEMean3.Fill(channel1-64, channel2-64, gausle1->GetParameter(1)+gausle2->GetParameter(1));
      beamHistoLESigma3.Fill(channel1-64, channel2-64, gausle1->GetParameter(2)+gausle2->GetParameter(2));

      beamHistoTEMean3.Fill(channel1-64, channel2-64, gauste1->GetParameter(1)+gauste2->GetParameter(1));
      beamHistoTESigma3.Fill(channel1-64, channel2-64, gauste1->GetParameter(2)+gauste2->GetParameter(2));

      timeDiffMean3.Fill(channel1-64, channel2-64, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma3.Fill(channel1-64, channel2-64, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
    }
  }

  beamHistoLGMean1.Write();
  beamHistoLGSigma1.Write();
  beamHistoHGMean1.Write();
  beamHistoHGSigma1.Write();
  timeDiffMean1.Write();
  timeDiffSigma1.Write();
  beamHistoLEMean1.Write();
  beamHistoLESigma1.Write();
  beamHistoTEMean1.Write();
  beamHistoTESigma1.Write();

  beamHistoLGMean2.Write();
  beamHistoLGSigma2.Write();
  beamHistoHGMean2.Write();
  beamHistoHGSigma2.Write();
  timeDiffMean2.Write();
  timeDiffSigma2.Write();
  beamHistoLEMean2.Write();
  beamHistoLESigma2.Write();
  beamHistoTEMean2.Write();
  beamHistoTESigma2.Write();

  beamHistoLGMean3.Write();
  beamHistoLGSigma3.Write();
  beamHistoHGMean3.Write();
  beamHistoHGSigma3.Write();
  timeDiffMean3.Write();
  timeDiffSigma3.Write();
  beamHistoLEMean3.Write();
  beamHistoLESigma3.Write();
  beamHistoTEMean3.Write();
  beamHistoTESigma3.Write();

/*
  for(int i=0; i<16; i++)
  {
    //lgAmp1[i]->Write();
    //lgAmp2[i]->Write();
    //hgAmp1[i]->Write();
    //hgAmp2[i]->Write();
    timeDiff1[i]->Write();
    timeDiff2[i]->Write();
  }
*/
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
  delete dataBuff;
  return 0;
}



