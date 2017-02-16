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

#include <stdio.h>
#include <string.h>
#include <exception>
#include <fstream>

#include "TFile.h"
#include "TH1D.h"

#include "tools.h"
#include "MDevent.h"
#include "MDfragmentBM.h"
#include "MDdataWordBM.h"
#include "MDpartEventBM.h"
#include "MDfileManager.h"
#include "MDexception.h"

using namespace std;

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTIONS] \n\n", argv);
  fprintf(stderr, "    -f / --file       <string>  ( Run number or name of a file ) [ required ]\n");
  fprintf(stderr, "    -d / --directory  <string>  ( Path for a data file)          [ optional ]\n");
}

uint32_t* dataPtr;

int main( int argc, char **argv ) {
  // Check the user arguments consistancy
  // All mandatory arguments should be provided.
  int file_arg   = get_arg_val('f', "file",      argc, argv);
  int dir_arg    = get_arg_val('d', "directory", argc, argv);

  if (file_arg == 0) {
    print_usage(argv[0]);
    return 1;
  }

  string filename = string( argv[file_arg] );
  string filepath(".");

  if (dir_arg != 0)
    filepath = string( argv[dir_arg] );

  MDfileManager fileManager(filename, filepath);
// Open the file and loop over events.
  char *eventBuffer;
  if ( fileManager.Open() ) { // There is a valid files to unpack.

    // ROOT histograms
    TFile rfile("histos.root", "recreate");
    TH1I  h_lgah("h_lg_amp_hit_ch", "hit channels", 100, 0, 100);
    TH1I  h_hgah("h_lh_amp_hit_ch", "hit channels", 100, 0, 100);
    TH1I  h_lth("h_le_time_hit_ch", "hit channels", 100, 0, 100);
    TH1I  h_tth("h_te_time_hit_ch", "hit channels", 100, 0, 100);
    TH1I  h_lga("h_lg_ampl", "hit ampl.", 200, 0, 5000);
    TH1I  h_hga("h_hg_ampl", "hit ampl.", 200, 0, 5000);

    int xEv(0);
//     while (xEv < 5) { // Unpack 5 spills.
    while (1) { // Loop over all spills.
      eventBuffer =  fileManager.GetNextEvent();
      if (!eventBuffer)
        break;

      try {
        MDevent  spill;
        spill.SetDataPtr(eventBuffer);
        cout << "\ndata: " << (void*) spill.GetDataPtr() << "  size: " << spill.GetSize() << endl;
        cout << "event: " << spill.HeaderPtr()->event_id_ << "  time: " << asctime( localtime(&spill.HeaderPtr()->time_) );
        cout << "fragments: " << spill.HeaderPtr()->n_fragments_ << "  type: " << (int) spill.HeaderPtr()->type_ << endl;

        int nFr = spill.HeaderPtr()->n_fragments_;
        uint8_t *dataPtr = spill.PayLoadPtr();
        for (int xFr=0; xFr<nFr; ++xFr) {
          MDfragmentBM board;
          board.SetDataPtr(dataPtr);
          dataPtr += board.GetSize();
          int nTr = board.GetNumOfTriggers();
          cerr << "number of triggers: " << nTr << endl;
          // Loop over all trigger events.
          for (int xTr=0; xTr<nTr; ++xTr) {
            MDpartEventBM *trEv = board.GetTriggerEventPtr(xTr);
//             trEv->Dump();
            for (int ich=0; ich<BM_FEB_NCHANNELS; ++ich) {
              int nHits = trEv->GetNLeadingEdgeHits(ich);
              if (nHits)
                h_lth.Fill(ich, nHits);

              nHits = trEv->GetNTrailingEdgeHits(ich);
              if (nHits)
                h_tth.Fill(ich, nHits);

              if (trEv->LGAmplitudeHitExists(ich)) {
                int q = trEv->GetHitAmplitude(ich, 'l');
                h_lga.Fill(q);
                h_lgah.Fill(ich);
              }

              if (trEv->HGAmplitudeHitExists(ich)) {
                int q = trEv->GetHitAmplitude(ich, 'h');
                h_hga.Fill(q);
                h_hgah.Fill(ich);
              }
            }
          }
        }
      } catch (MDexception & lExc)  {
        std::cerr <<  lExc.GetDescription() << endl
                  << "Unpacking exception\n"
                  << "Spill skipped!\n\n";
      }

      ++xEv;
    }

    h_lgah.Write();
    h_hgah.Write();
    h_lth.Write();
    h_tth.Write();
    h_lga.Write();
    h_hga.Write();

    rfile.Close();
  }

  delete eventBuffer;
  return 0;
}



