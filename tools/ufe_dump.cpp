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

#include "tools.h"
#include "MDdataWordBM.h"

using namespace std;

void print_usage(char *argv) {
  fprintf(stderr, "\nUsage: %s [OPTIONS] \n\n", argv);
  fprintf(stderr, "    -f / --file           <string>       ( Name of a data file )                  [ required ]\n");
  fprintf(stderr, "    -d / --directory      <string>       ( Path for a data file)                  [ optional ]\n");
  fprintf(stderr, "    -b / --begin          <int hex/dec>  ( Initial position in input file )       [ optional ]\n");
  fprintf(stderr, "    -n / --n-words    <int hex/dec>  ( Number of data words to be processed ) [ optional ]\n");
}


int main( int argc, char **argv ) {
  // Check the user arguments consistancy
  // All mandatory arguments should be provided.
  int file_arg   = get_arg_val('f', "file"     , argc, argv);
  int dir_arg    = get_arg_val('d', "directory", argc, argv);
  int begin_arg  = get_arg_val('b', "begin"    , argc, argv);
  int nwords_arg = get_arg_val('n', "n-words"  , argc, argv);

  if (file_arg == 0) {
    print_usage(argv[0]);
    return 1;
  }

  string filename = string( argv[file_arg] );
  string filepath(".");

  if (dir_arg != 0)
    filepath = string( argv[dir_arg] );

  std::ifstream ifs((filepath + "/" + filename).c_str());
  if ( ifs.fail() ) {
    cerr << "Can not open file " << filepath << filename << endl;
    return 1;
  }

  size_t pos(0);
  if (begin_arg != 0)
    pos = arg_as_int( argv[begin_arg] );

  int nWords(0);
  if (nwords_arg != 0)
    nWords = arg_as_int( argv[nwords_arg] );

  char* dataPtr = new char[4];
  ifs.seekg(pos*4);

  int dwCount(0);
  while ( nWords == 0 || dwCount < nWords ) {
    ifs.read(dataPtr, 4 );
    if (ifs.eof())
      break;

    MDdataWordBM dw(dataPtr);
    cout << dw << endl;

    ++dwCount;
  }

  ifs.close();
  delete dataPtr;
  return 0;
}


