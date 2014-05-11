#include <iostream>
#include <string>
#include <cmath>
#include <stdlib.h>

#include "TRandom3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TROOT.h"

#include <swat/TAuxFunc.h>
#include <swat/TSourcesFinder.h>
#include <swat/TAnalysis.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Searches for multiplets in herald data or CRPropa simulations. Calculates\n\
   correlation of events hitting stripe on the tangent plane and dispalys on\n\
   the screen. The results are saved to sources.root file. Two input file formats\n\
   are supported, both are TTrees saved in a .root file. The TTrees can be either\n\
   the output of CRPropa or a Herald file converted to TTree (see macro\n\
   macros/convert_herald.C in swat source tree).\n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-i emin] [-e emax] [-w width]\n\
                         [-l length] [-f file.root] [-n nsources] [-t wav_threshold]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     Wavelet scale. It is a number in the range 0 <= j <= 8, defaults to 1.\n\
   -N:     Band limit of wavelet, in the range 0 < N <= 128, defaults to 1.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -a:     Minimum time in unix timestamp.\n\
   -b:     Maximum time in unix timestamp.\n\
   -w:     Width of tangent plane, defaults to 2 degrees.\n\
   -l:     Length of tangent plane, defaults to 10 degrees.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   -n:     Number of sources to look for. Default to 15\n\
   -t:     Wavelet threshold value.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   // Needed this to inform root we need dictionaries for std::complex.
   gROOT->ProcessLine("#include <complex>");

   int N = 1, j = 1, n = 15;
   double w = 3., width = 2., length = 10., min = 20., max = 40., wavt = 0;
   string emin = "20", emax = "40", file = "chain.root", sources = "sources", tmax = "1072959037", tmin = "1072959036";

   char opt;

   while ((opt = getopt(argc,argv,"+hj:m:N:n:i:e:w:l:c:s:f:t:a:b:")) != -1) {
      switch (opt) {
         case 'j': 
	    j = atoi(optarg);
	    sources += "j";
	    sources += optarg;
	    break;
         case 'n': 
	    n = atoi(optarg);
	    break;
         case 'i':
	    emin = optarg;
	    min = atof(optarg);
	    sources += "i";
	    sources += optarg;
	    break;
         case 'a':
	    tmin = optarg;
	    break;
         case 'b':
	    tmax = optarg;
	    break;
         case 'e':
	    emax = optarg;
	    max = atof(optarg);
	    sources += "e";
	    sources += optarg;
	    break;
         case 'f':
	    file = optarg;
	    break;
         case 'w':
	    width = atof(optarg);
	    sources += "w";
	    sources += optarg;
	    break;
         case 'l':
	    length = atof(optarg);
	    sources += "l";
	    sources += optarg;
	    break;
         case 't':
	    wavt = atof(optarg);
	    break;
         case 'N':
	    if ((N = atoi(optarg)) < 1) {
	       cerr << "N = " << N << ", N must be greaer than 0" << endl;
	       exit(EXIT_FAILURE);
	    }
	    sources += "N";
	    sources += optarg;
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   sources += ".root";
   TFile sourcesfile(sources.c_str(),"recreate");

   if (sourcesfile.IsZombie()) {
      cerr << "TFile: Invalid file." << endl;
      exit(EXIT_FAILURE);
   }

   TFile addfile(file.c_str());

   if (addfile.IsZombie()) {
      cerr << "TFile: Invalid file." << endl;
      exit(EXIT_FAILURE);
   }

   addfile.cd(); // Redundant but useful.
   TSourcesFinder finder;
   finder.SetMinEnergy(emin.c_str());
   finder.SetMaxEnergy(emax.c_str());
   finder.SetMinTime(tmin.c_str());
   finder.SetMaxTime(tmax.c_str());
   finder.SetN(N);
   finder.SetScale(j);
   finder.SetNSources(n);
   finder.SetSeparation(w);
   sourcesfile.cd();
   WavStat stat = finder.FindSources(wavt);

   cout << "\nWavelet statistics:\n\n"
        << "   - Greatest: " << stat.biggest << "\n"
        << "   - Mean: "     << stat.mean << "\n"
        << "   - Variance: " << stat.variance << "\n"
        << "   - Skewness: " << stat.skewness << "\n"
        << "   - Kurtosis: " << stat.kurtosis << "\n"
	<< endl;

   cout << "\nMagnitude for the first sources:\n\n";
   cout << "W = ";
   for (int i = 0; i < stat.mags.size() - 1; ++i)
     std::cout << stat.mags[i] << ", ";

   std::cout << stat.mags[stat.mags.size() - 1] << endl;

   TAnalysis analysis(&sourcesfile,&addfile);
   analysis.SetLength(length/2);
   analysis.SetWidth(width/2);
   analysis.GenDeflectionGraphs();
   gDirectory->ls();
   gDirectory->Write();

   return 0;
}

