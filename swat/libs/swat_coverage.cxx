#include <iostream>
#include <string>
#include <cmath>
#include "TRandom3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TROOT.h"
#include <stdlib.h>

#include <swat/swat/TAuxFunc.h>
#include <swat/swat/TSourcesFinder.h>
#include <swat/swat/TSkyMap.h>
#include <swat/swat/TVMap.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Uses a set of events to generate a coverage map based on the Spherical\n\
   Wavelet Transform. The steps follwed are:\n\n\
   1 - A sky map f is calculated in the Healpix tesselation.\n\
   2 - We calculate the harmonic expansion of f, resulting in the coefficientes a_lm\n\
   3 - From a_lm, we calculate the wavelet representation of the map W.\n\
   4 - Filtering is performed on W (scales less than j are removed.)\n\
   5 - The backward transform is calculated from W, resulting in filtered a_lm.\n\
   6 - The backward spherical transform is used to calculate the filtered f,\n\
       in ECP tesselation.\n\
   7 - A histogram with the coverage is the final output (histogram theta vs phi)\n\
   \n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-i emin] [-e emax] [-w width]\n\
                         [-l length] [-f file.root] [-n nsources]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     Wavelet scale. It is a number in the range 0 <= j <= 8, defaults to 4.\n\
   -N:     Band limit of wavelet, in the range 0 < N <= 128, defaults to 1.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   // Needed this to inform root we need dictionaries for std::complex.
   gROOT->ProcessLine("#include <complex>");

   int N = 1, j = 4;
   double w = 3., min = 20., max = 40.;
   string emin = "20", emax = "40", file = "chain.root", sources = "coverage";

   char opt;

   while ((opt = getopt(argc,argv,"+hj:N:i:e:f:")) != -1) {
      switch (opt) {
         case 'j': 
	    j = atoi(optarg);
	    sources += "j";
	    sources += optarg;
	    break;
         case 'i':
	    emin = optarg;
	    min = atof(optarg);
	    sources += "i";
	    sources += optarg;
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
   finder.SetN(N);
   finder.SetScale(j);
   finder.SetSeparation(w);
   finder.GenerateAlm(); // Add alm to the current directory.

   TAlm* alm = dynamic_cast<TAlm*>(gDirectory->Get("alm"));
   if (!alm) {
      std::cerr << "TSourcesFinder::FindSources: Unalbe to retrieve alm." << std::endl;
      throw 1;
   }

   const TAlm* alm2 = TAuxFunc::alm2wav2alm(*alm, N, j, 8);
   const TVMap* sky = TAuxFunc::SHT(*alm2, false);
   delete alm2;

   sourcesfile.cd();
   const TH2D* coverage = sky->CreateHist(0);
   gDirectory->Write();

   return 0;
}

