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
  TFile rfile("beamAnalysis.root", "recreate");
  TH1I  h_lgah("h_lg_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_hgah("h_lh_amp_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lth("h_le_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_tth("h_te_time_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h_lga("h_lg_ampl", "hit ampl.", 200, 0, 5000);
  TH1I  h_hga("h_hg_ampl", "hit ampl.", 200, 0, 5000);

  TH1I  timeStudy4("timeStudy4", "timeStudy4", 20, -10, 10);
  TH1I  timeStudy8("timeStudy8", "timeStudy8", 2000, -10, 10);
  TH1I  timeStudy12("timeStudy12", "timeStudy12", 20, -10, 10);

  TH1I  timeStudy80("timeStudy80", "timeStudy80", 2000, -10, 10);
  TH1I  timeStudy81("timeStudy81", "timeStudy81", 2000, -10, 10);
  TH1I  timeStudy82("timeStudy82", "timeStudy82", 2000, -10, 10);
  TH1I  timeStudy83("timeStudy83", "timeStudy83", 2000, -10, 10);
  TH1I  timeStudy84("timeStudy84", "timeStudy84", 2000, -10, 10);
  TH1I  timeStudy85("timeStudy85", "timeStudy85", 2000, -10, 10);
  TH1I  timeStudy86("timeStudy86", "timeStudy86", 2000, -10, 10);
  TH1I  timeStudy87("timeStudy87", "timeStudy87", 2000, -10, 10);
  TH1I  timeStudy88("timeStudy88", "timeStudy88", 2000, -10, 10);
  TH1I  timeStudy89("timeStudy89", "timeStudy89", 2000, -10, 10);
  TH1I  timeStudy810("timeStudy810", "timeStudy810", 2000, -10, 10);
  TH1I  timeStudy811("timeStudy811", "timeStudy811", 2000, -10, 10);
  TH1I  timeStudy812("timeStudy812", "timeStudy812", 2000, -10, 10);
  TH1I  timeStudy813("timeStudy813", "timeStudy813", 2000, -10, 10);
  TH1I  timeStudy814("timeStudy814", "timeStudy814", 2000, -10, 10);
  TH1I  timeStudy815("timeStudy815", "timeStudy815", 2000, -10, 10);


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

      //if(counter==10) break;

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

        for (int i=0; i<nTr; ++i) {

          event = spill.GetTriggerEventPtr(i);
          event2 = spill2.GetTriggerEventPtr(i);

          if(event->getNumDataWords() < 3 || event2->getNumDataWords() < 3) continue;

          // Beam profile.
          //cout<<"trigger"<<i<<endl;
          for (int channel1=0; channel1<BM_FEB_NCHANNELS-16; channel1++)
          {
            if(event->LGAmplitudeHitExists(channel1) && event->LGAmplitudeHitExists(channel1+16))
            {
              for (int channel2=0; channel2<16; channel2++)
              {
                if(event2->LGAmplitudeHitExists(channel2) && event2->LGAmplitudeHitExists(channel2+16))
                {
                  // Do for all channel 2 and channel1 =8
                  int num = 4;

                  if(channel1==4 && channel2==8) timeStudy4.Fill((int)event2->GetHitTime(0,8, 'l')-(int)event2->GetHitTime(0,8+16, 'l')); 
                   if(channel1==8 && channel2==num) timeStudy8.Fill((int)event2->GetHitTime(0,num, 'l')-(int)event2->GetHitTime(0,num+16, 'l')); 
                  if(channel1==12 && channel2==8) timeStudy12.Fill((int)event2->GetHitTime(0,8, 'l')-(int)event2->GetHitTime(0,8+16, 'l'));
                
                  if(channel1==8 && channel2==0) timeStudy80.Fill((int)event2->GetHitTime(0,0, 'l')-(int)event2->GetHitTime(0,0+16, 'l')); 
                  if(channel1==8 && channel2==1) timeStudy81.Fill((int)event2->GetHitTime(0,1, 'l')-(int)event2->GetHitTime(0,1+16, 'l')); 
                  if(channel1==8 && channel2==2) timeStudy82.Fill((int)event2->GetHitTime(0,2, 'l')-(int)event2->GetHitTime(0,2+16, 'l')); 
                  if(channel1==8 && channel2==3) timeStudy83.Fill((int)event2->GetHitTime(0,3, 'l')-(int)event2->GetHitTime(0,3+16, 'l')); 
                  if(channel1==8 && channel2==4) timeStudy84.Fill((int)event2->GetHitTime(0,4, 'l')-(int)event2->GetHitTime(0,4+16, 'l')); 
                  if(channel1==8 && channel2==5) timeStudy85.Fill((int)event2->GetHitTime(0,5, 'l')-(int)event2->GetHitTime(0,5+16, 'l')); 
                  if(channel1==8 && channel2==6) timeStudy86.Fill((int)event2->GetHitTime(0,6, 'l')-(int)event2->GetHitTime(0,6+16, 'l')); 
                  if(channel1==8 && channel2==7) timeStudy87.Fill((int)event2->GetHitTime(0,7, 'l')-(int)event2->GetHitTime(0,7+16, 'l')); 
                  if(channel1==8 && channel2==8) timeStudy88.Fill((int)event2->GetHitTime(0,8, 'l')-(int)event2->GetHitTime(0,8+16, 'l')); 
                  if(channel1==8 && channel2==9) timeStudy89.Fill((int)event2->GetHitTime(0,9, 'l')-(int)event2->GetHitTime(0,9+16, 'l')); 
                  if(channel1==8 && channel2==10) timeStudy810.Fill((int)event2->GetHitTime(0,10, 'l')-(int)event2->GetHitTime(0,10+16, 'l')); 
                  if(channel1==8 && channel2==11) timeStudy811.Fill((int)event2->GetHitTime(0,11, 'l')-(int)event2->GetHitTime(0,11+16, 'l')); 
                  if(channel1==8 && channel2==12) timeStudy812.Fill((int)event2->GetHitTime(0,12, 'l')-(int)event2->GetHitTime(0,12+16, 'l')); 
                  if(channel1==8 && channel2==13) timeStudy813.Fill((int)event2->GetHitTime(0,13, 'l')-(int)event2->GetHitTime(0,13+16, 'l')); 
                  if(channel1==8 && channel2==14) timeStudy814.Fill((int)event2->GetHitTime(0,14, 'l')-(int)event2->GetHitTime(0,14+16, 'l')); 
                  if(channel1==8 && channel2==15) timeStudy815.Fill((int)event2->GetHitTime(0,15, 'l')-(int)event2->GetHitTime(0,15+16, 'l')); 

                }

                if(event2->LGAmplitudeHitExists(channel2) && event2->LGAmplitudeHitExists(channel2+16)) beamHistoLG1.Fill(channel1, channel2, 1);

                if(event2->LGAmplitudeHitExists(channel2+32) && event2->LGAmplitudeHitExists(channel2+32+16)) beamHistoLG2.Fill(channel1-32, channel2, 1);

                if(event2->LGAmplitudeHitExists(channel2+64) && event2->LGAmplitudeHitExists(channel2+64+16)) beamHistoLG3.Fill(channel1-64, channel2, 1);
              }
            }
            if(event->HGAmplitudeHitExists(channel1) && event->HGAmplitudeHitExists(channel1+16))
            {
              for (int channel2=0; channel2<16; channel2++)
              {
                if(event2->HGAmplitudeHitExists(channel2) && event2->HGAmplitudeHitExists(channel2+16)) beamHistoHG1.Fill(channel1, channel2, 1);

                if(event2->HGAmplitudeHitExists(channel2+32) && event2->HGAmplitudeHitExists(channel2+32+16)) beamHistoHG2.Fill(channel1-32, channel2, 1);
  
                if(event2->HGAmplitudeHitExists(channel2+64) && event2->HGAmplitudeHitExists(channel2+64+16)) beamHistoHG3.Fill(channel1-64, channel2, 1);
              }
          }
        }
          for (int channel=0; channel<BM_FEB_NCHANNELS-16; channel++)
          {
            if(event->LGAmplitudeHitExists(channel) && event->LGAmplitudeHitExists(channel+16)) lgAmp1[channel]->Fill(event->GetHitAmplitude(channel, 'l')+event->GetHitAmplitude(channel+16, 'l'));
            if(event2->LGAmplitudeHitExists(channel) && event2->LGAmplitudeHitExists(channel+16)) lgAmp2[channel]->Fill(event2->GetHitAmplitude(channel, 'l')+event2->GetHitAmplitude(channel+16, 'l'));
            if(event->HGAmplitudeHitExists(channel) && event->HGAmplitudeHitExists(channel+16)) hgAmp1[channel]->Fill(event->GetHitAmplitude(channel, 'h')+event->GetHitAmplitude(channel+16, 'h'));
            if(event2->HGAmplitudeHitExists(channel) &&event2->HGAmplitudeHitExists(channel+16)) hgAmp2[channel]->Fill(event2->GetHitAmplitude(channel, 'h')+event2->GetHitAmplitude(channel+16, 'h'));
          
            le1[channel]->Fill(event->GetNLeadingEdgeHits(channel)+event->GetNLeadingEdgeHits(channel+16));
            le2[channel]->Fill(event2->GetNLeadingEdgeHits(channel)+event2->GetNLeadingEdgeHits(channel+16));
            te1[channel]->Fill(event->GetNTrailingEdgeHits(channel)+event->GetNTrailingEdgeHits(channel+16));
            te2[channel]->Fill(event2->GetNTrailingEdgeHits(channel)+event2->GetNTrailingEdgeHits(channel+16));
          }
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

              //int nFirstHits2 = event2->GetNLeadingEdgeHits(channel+inChannel);
              //int nSecondHits2 = event2->GetNLeadingEdgeHits(channel+inChannel+16);

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

          for(int vecPos = 0; vecPos<nFirstHitsVec.size(); vecPos++)
          {
            for(int iFirsth=0; iFirsth<nFirstHitsVec[vecPos]; iFirsth++)
            {
              for(int iSecondh=0; iSecondh<nSecondHitsVec[vecPos]; iSecondh++)
              {
                if(event->GetHitTime(iFirsth,channelVec[vecPos]+inChannelVec[vecPos], 'l') != 0 &&
                  event->GetHitTime(iSecondh,channelVec[vecPos]+inChannelVec[vecPos]+16, 'l') != 0 )
                {
                  int channelSum = channelVec[vecPos]+inChannelVec[vecPos];
                  timeDiff1[channelSum]->Fill((double)event->GetHitTime(iFirsth,channelSum, 'l')-
                    (double)event->GetHitTime(iSecondh,channelSum+16, 'l'));
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
                }
              }
            }
          }

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

  for (int channel1=0; channel1<BM_FEB_NCHANNELS; channel1++)
  {
    if((16<channel1&&channel1<32) || (48<channel1&&channel1<64) || (80<channel1&&channel1<96)) continue;

    for (int channel2=0; channel2<BM_FEB_NCHANNELS; channel2++)
    {
      if((16<channel2&&channel2<32) || (48<channel2&&channel2<64) || (80<channel2&&channel2<96)) continue;

      double gauslg1Mean =0;
      double gauslg2Mean =0;
      double gauslg1Sigma =0;
      double gauslg2Sigma =0;

      if(lgAmp1[channel1]->GetEntries()>10 && lgAmp2[channel2]->GetEntries()>10)
      {
      lgAmp1[channel1]->Fit("gaus","Q");
      TF1* gauslgAmp1 = lgAmp1[channel1]->GetFunction("gaus");
      lgAmp2[channel2]->Fit("gaus","Q");
      TF1* gauslgAmp2 = lgAmp2[channel2]->GetFunction("gaus");

      gauslg1Mean=gauslgAmp1->GetParameter(1);
      gauslg2Mean=gauslgAmp2->GetParameter(1);
      gauslg1Sigma=gauslgAmp1->GetParameter(1);
      gauslg2Sigma=gauslgAmp2->GetParameter(2);
      }
      
      double gaushg1Mean =0;
      double gaushg2Mean =0;
      double gaushg1Sigma =0;
      double gaushg2Sigma =0;

      if(hgAmp1[channel1]->GetEntries()>10 && hgAmp2[channel2]->GetEntries()>10)
      {
      hgAmp1[channel1]->Fit("gaus","Q");
      TF1* gaushgAmp1 = hgAmp1[channel1]->GetFunction("gaus");
      hgAmp2[channel2]->Fit("gaus","Q");
      TF1* gaushgAmp2 = hgAmp2[channel2]->GetFunction("gaus");

      gaushg1Mean=gaushgAmp1->GetParameter(1);
      gaushg2Mean=gaushgAmp2->GetParameter(1);
      gaushg1Sigma=gaushgAmp1->GetParameter(1);
      gaushg2Sigma=gaushgAmp2->GetParameter(2);
      }

      double gausle1Mean =0;
      double gausle2Mean =0;
      double gausle1Sigma =0;
      double gausle2Sigma =0;

      if(le1[channel1]->GetEntries()>10 && le2[channel2]->GetEntries()>10)
      {
      le1[channel1]->Fit("gaus","Q");
      TF1* gausle1 = le1[channel1]->GetFunction("gaus");
      le2[channel2]->Fit("gaus","Q");
      TF1* gausle2 = le2[channel2]->GetFunction("gaus");

      gausle1Mean=gausle1->GetParameter(1);
      gausle2Mean=gausle2->GetParameter(1);
      gausle1Sigma=gausle1->GetParameter(1);
      gausle2Sigma=gausle2->GetParameter(2);
      }

      double gauste1Mean =0;
      double gauste2Mean =0;
      double gauste1Sigma =0;
      double gauste2Sigma =0;

      if(te1[channel1]->GetEntries()>10 && te2[channel2]->GetEntries()>10)
      {
      te1[channel1]->Fit("gaus","Q");
      TF1* gauste1 = te1[channel1]->GetFunction("gaus");
      te2[channel2]->Fit("gaus","Q");
      TF1* gauste2 = te2[channel2]->GetFunction("gaus");

      gauste1Mean=gauste1->GetParameter(1);
      gauste2Mean=gauste2->GetParameter(1);
      gauste1Sigma=gauste1->GetParameter(1);
      gauste2Sigma=gauste2->GetParameter(2);
      }

      double gaustimeDiff1Mean =4;
      double gaustimeDiff2Mean =4;
      double gaustimeDiff1Sigma =4;
      double gaustimeDiff2Sigma =4;

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
      beamHistoLGMean1.Fill(channel1, channel2, gauslg1Mean+gauslg2Mean);
      beamHistoLGSigma1.Fill(channel1, channel2, gauslg1Sigma+gauslg2Sigma);
      
      beamHistoHGMean1.Fill(channel1, channel2, gaushg1Mean+gaushg2Mean);
      beamHistoHGSigma1.Fill(channel1, channel2, gaushg1Sigma+gaushg2Sigma);
    
      beamHistoLEMean1.Fill(channel1, channel2, gausle1Mean+gausle2Mean);
      beamHistoLESigma1.Fill(channel1, channel2, gausle1Sigma+gausle2Sigma);

      beamHistoTEMean1.Fill(channel1, channel2, gauste1Mean+gauste2Mean);
      beamHistoTESigma1.Fill(channel1, channel2, gauste1Sigma+gauste2Sigma);

      timeDiffMean1.Fill(channel1, channel2, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma1.Fill(channel1, channel2, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
      else if(32<channel1 && channel1<48 && 32<channel2 && channel2<48)
      {
      beamHistoLGMean2.Fill(channel1-32, channel2-32, gauslg1Mean+gauslg2Mean);
      beamHistoLGSigma2.Fill(channel1-32, channel2-32, gauslg1Sigma+gauslg2Sigma);
      
      beamHistoHGMean2.Fill(channel1-32, channel2-32, gaushg1Mean+gaushg2Mean);
      beamHistoHGSigma2.Fill(channel1-32, channel2-32, gaushg1Sigma+gaushg2Sigma);
    
      beamHistoLEMean2.Fill(channel1-32, channel2-32, gausle1Mean+gausle2Mean);
      beamHistoLESigma2.Fill(channel1-32, channel2-32, gausle1Sigma+gausle2Sigma);

      beamHistoTEMean2.Fill(channel1-32, channel2-32, gauste1Mean+gauste2Mean);
      beamHistoTESigma2.Fill(channel1-32, channel2-32, gaustimeDiff1Sigma+gaustimeDiff2Sigma);

      timeDiffMean2.Fill(channel1-32, channel2-32, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma2.Fill(channel1-32, channel2-32, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
      else if(64<channel1 && channel1<80 && 64<channel2 && channel2<80)
      {
      beamHistoLGMean3.Fill(channel1-64, channel2-64, gauslg1Mean+gauslg2Mean);
      beamHistoLGSigma3.Fill(channel1-64, channel2-64, gauslg1Sigma+gauslg2Sigma);

      beamHistoHGMean3.Fill(channel1-64, channel2-64, gaushg1Mean+gaushg2Mean);
      beamHistoHGSigma3.Fill(channel1-64, channel2-64, gaushg1Sigma+gaushg2Sigma);
    
      beamHistoLEMean3.Fill(channel1-64, channel2-64, gausle1Mean+gausle2Mean);
      beamHistoLESigma3.Fill(channel1-64, channel2-64, gausle1Sigma+gausle2Sigma);

      beamHistoTEMean3.Fill(channel1-64, channel2-64, gauste1Mean+gauste2Mean);
      beamHistoTESigma3.Fill(channel1-64, channel2-64, gaustimeDiff1Sigma+gaustimeDiff2Sigma);

      timeDiffMean3.Fill(channel1-64, channel2-64, gaustimeDiff1Mean+gaustimeDiff2Mean);
      timeDiffSigma3.Fill(channel1-64, channel2-64, gaustimeDiff1Sigma+gaustimeDiff2Sigma);
      }
    }
  }

timeStudy4.Write();
timeStudy8.Write();
timeStudy12.Write();

timeStudy80.Write();
timeStudy81.Write();
timeStudy82.Write();
timeStudy83.Write();
timeStudy84.Write();
timeStudy85.Write();
timeStudy86.Write();
timeStudy87.Write();
timeStudy88.Write();
timeStudy89.Write();
timeStudy810.Write();
timeStudy811.Write();
timeStudy812.Write();
timeStudy813.Write();
timeStudy814.Write();
timeStudy815.Write();

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



