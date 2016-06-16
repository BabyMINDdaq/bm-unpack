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

#include "MDfragmentBM.h"
#include "MDpartEventBM.h"
#include "MDargumentHandler.h"
#include "MDdataFile.h"

using namespace std;

char *dataBuff;
uint32_t* dataPtr;

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

//   std::ifstream ifs((filepath + "/" + filename).c_str());
//   if ( ifs.fail() ) {
//     cerr << "Can not open file " << filepath << filename << endl;
//     return 1;
//   }

//   dataBuff = new char[64*1024*4];
//   dataPtr  = (uint32_t*)dataBuff;
// 
//   ifs.read(dataBuff, 5*4);

  TFile rfile("histos.root", "recreate");
  TH1I  h1("h1_hit_ch", "hit channels", 100, 0, 100);
  TH1I  h2("h2_ampl", "hit ampl.", 200, 0, 1000);

  MDdateFile dfile(filename, filepath);
// Open the file and loop over events.

  try {

    char *eventBuffer;
    if ( dfile.open() ) { // There is a valid files to unpack
      dfile.init();

      int xEv(0);
       do { // Loop over all spills
        eventBuffer =  dfile.GetNextEvent();
        MDfragmentBM   spill;
        spill.SetDataPtr(eventBuffer);

        MDpartEventBM  event;
        int nTr = spill.GetNumOfTriggers();
        for (int i=0; i<nTr; ++i) {
          uint32_t *trigPtr = spill.GetTriggerEventPtr(i);
          event.SetDataPtr(trigPtr);
          for (int ich=0; ich<BM_FEB_NCHANNELS; ++ich) {
            int nHits = event.GetNLeadingEdgeHits(ich);
            if (nHits)
              h1.Fill(ich, nHits);

            int q = event.GetHitAmplitude(ich, 'h');
            h2.Fill(q);
          }
        }
        ++xEv;
//       } while (xEv < 5);
      } while ( eventBuffer );
    }

    dfile.close();
  } catch (MDexception & lExc)  {
    std::cerr <<  lExc.GetDescription() << endl;
    std::cerr << "Unpacking exception" << std::endl;// return 1;
  } catch(std::exception & lExc) {
    std::cerr << "Standard exception" << std::endl;
    std::cerr << lExc.what() << std::endl;
  } catch(...) {
    std::cerr << "Unknown exception occurred..." << std::endl;
  }

  h1.Write();
  h2.Write();
  rfile.Close();
  dfile.close();
  delete dataBuff;
  return 0;
}



