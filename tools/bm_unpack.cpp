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
#include "MDdataFile.h"
#include "MDexception.h"

using namespace std;

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTIONS] \n\n", argv);
  fprintf(stderr, "    -f / --file           <string>       ( Name of a data file )                  [ required ]\n");
  fprintf(stderr, "    -d / --directory      <string>       ( Path for a data file)                  [ optional ]\n");
}

uint32_t* dataPtr;

int main( int argc, char **argv ) {
  // Check the user arguments consistancy
  // All mandatory arguments should be provided.
  int file_arg   = get_arg_val('f', "file"    , argc, argv);
  int dir_arg    = get_arg_val('d', "directory" , argc, argv);

  if (file_arg == 0) {
    print_usage(argv[0]);
    return 1;
  }

  string filename = string( argv[file_arg] );
  string filepath(".");

  if (dir_arg != 0)
    filepath = string( argv[dir_arg] );

  MDdateFile dfile(filename, filepath);
// Open the file and loop over events.
  char *eventBuffer;
  if ( dfile.open() ) { // There is a valid files to unpack
    int xEv(0);
    while (1) { // Loop over all spills
//     while (xEv < 5);
      eventBuffer =  dfile.GetNextEvent();
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
          int nTr = board.GetNumOfTriggers();
          cerr << "number of triggers: " << nTr << endl;
          dataPtr += board.GetSize();
        }
      } catch (MDexception & lExc)  {
        std::cerr <<  lExc.GetDescription() << endl
                  << "Unpacking exception\n"
                  << "Spill skipped!\n\n";
      }

      ++xEv;
    }
  }

  dfile.close();
  return 0;
}



