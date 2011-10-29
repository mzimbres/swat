// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 Marcelo Zimbres Silva
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>

// ROOT

// SWAT

#include "TAlm.h"
#include "TAuxFunc.h"
#include "TParser.h"

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* progname);

int main(int argc,const char* argv[])
{
   // The program reads TAlm object in file(first 
   // argument) and filters using thresholds saved 
   // in file(second argument). Filtered object 
   // will be output in filteredNN.root file.

   // Possible options

   TParser parser(argc,argv);

   string help("--help");
   if (parser.exist(help)){
      print_usage(argv[0]);
      return 0;
   } 

   int J = 6;
   string Jmax("-J");
   if (!parser.get<int>(J,Jmax)) {
      //cout << "Using J = 6" << endl;
   }

   TAlm *alm = TAuxFunc::rand_gaus_alm(J,0,1);

   int N = 3; 
   string bandlim("--wav-band-lim");
   if (!parser.get<int>(N,bandlim)) {
      //cout << "Using N = " << N << endl;
   }

   int ji = 0; 
   string jmin("--jmin");
   if (!parser.get<int>(ji,jmin)) {
      //cout << "No jmin specified, using 0" << endl;
   }

   int jm = J; 
   string jmax("--jmax");
   if (!parser.get<int>(jm,jmax)) {
      //cout << "No jmax specified, using J = " << J << endl;
   }

   TAlm *filtered = alm2wav2alm(*alm,N,ji,jm);
   if (filtered == NULL){
     cerr << "Unable to do the transform." << endl;
     return 1;
   } 

   if (!compare_alm(*alm,*filtered))
      return 1;

   delete filtered;
   delete alm;
   return 0;
}

void print_usage(const char* progname)
{
   cout << "\n";
   cout << "   Tests the algorithm doing a forward an backward transform.\n";
   cout << "\n";
   cout << "   Usage: " << progname 
        << " --signal=file1 --thresholds=file2 --wav-band-lim=N --output-file=file3\n";
   cout << "   Options:\n";
   cout << "\n";
   cout << "   --help:         This menu.\n";
   cout << "   -J:             TAlm object in root file. If not provided, the program \n"
        << "                   will compare input and output for J = 7 and return true if\n"
	<< "                   diffrence less that 10e-10. \n";
   cout << "   --wav-band-lim: Band limit of wavelet to be used, defaults to N = 3.\n";
   cout << "   --jmin:         Will begin the analysis at this scale, defaults to 0.\n";
   cout << "   --jmax:         Will end the analysis at this scale, defaults to alm.GetJmax().\n";
   cout << "\n";
}

