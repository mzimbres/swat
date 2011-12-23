#include <iostream>
#include <string>
#include <cmath>
#include "TRandom3.h"
#include "TF1.h"
#include "TAuxFunc.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"
#include "TGraph.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include <stdlib.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates sources from herald data, additionaly calculates correlation of\n\
   events hitting tangent plane and dispalys on the screen. The results are\n\
   saved to sources.root file. To convert a Herald file to the format used by\n\
   this progra use the macro macros/convert_herald.C in swat source tree.\n\
   Usage: " << prog << " [ -j scale] [-N number] [-i emin] [-e emax] [-w width]\n\
                         [-l length] [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     Wavelet scale a number in the range 0 <= j <= 8, defaults to 1.\n\
   -N:     Band limit of wavelet, in the range 0 < N <= 128, defaults to 1.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -w:     Width of tangent plane, defaults to 2 degrees.\n\
   -l:     Length of tangent plane, defaults to 10 degrees.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   int N = 1, j = 1;
   double w = 3., width = 2., length = 10., min = 20., max = 40.;
   string emin = "20", emax = "40", file = "chain.root";

   char opt;

   while ((opt = getopt(argc,argv,"+hj:m:N:n:i:e:w:l:c:s:f:")) != -1) {
      switch (opt) {
         case 'j': j = atoi(optarg);
	    break;
         case 'i':
	    emin = optarg;
	    min = atof(optarg);
	    break;
         case 'e':
	    emax = optarg;
	    max = atof(optarg);
	    break;
         case 'f':
	    file = optarg;
	    break;
         case 'w':
	    width = atof(optarg);
	    break;
         case 'l':
	    length = atof(optarg);
	    break;
         case 'N':
	    if ((N = atoi(optarg)) < 1) {
	       cerr << "N = " << N << ", N must be greaer than 0" << endl;
	       exit(EXIT_FAILURE);
	    }
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   TFile addfile(file.c_str());

   if (addfile.IsZombie()) {
      cerr << "TFile: Invalid file." << endl;
      exit(EXIT_FAILURE);
   }

   TSourcesFinder finder;
   finder.SetMinEnergy(emin.c_str());
   finder.SetMaxEnergy(emax.c_str());
   finder.SetN(N);
   finder.SetScale(j);
   finder.SetNSources(15);
   finder.SetSeparation(w);
   finder.FindSources();

   TAnalysis analysis(gDirectory,gDirectory);
   analysis.SetLength(length/2);
   analysis.SetWidth(width/2);
   analysis.GenDeflectionGraphs();
   gDirectory->ls();

   return 0;
}

