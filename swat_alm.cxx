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
#include "TROOT.h"
#include <stdlib.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Reads a herald file in Root format, applys cuts passed on command line\n\
   and calculates multipolar coefficients. The coefficients are saved on a\n\
   .root file. The output file name uses parameters passed on the command line.\n\n\
   Usage: " << prog << " [-i emin] [-e emax] [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   double min = 20., max = 40.;
   string emin = "20", emax = "40", file = "chain.root", sources = "alm";

   char opt;

   while ((opt = getopt(argc,argv,"+h:i:e:f:")) != -1) {
      switch (opt) {
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
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   sources += ".root";
   TFile addfile(file.c_str());

   if (addfile.IsZombie()) {
      cerr << "TFile: Invalid file." << endl;
      exit(EXIT_FAILURE);
   }

   addfile.cd(); // Redundant but useful.
   TSourcesFinder finder;
   finder.SetMinEnergy(emin.c_str());
   finder.SetMaxEnergy(emax.c_str());
   finder.GenerateAlm();

   TObject* alm = gDirectory->Get("alm");

   TFile sourcesfile(sources.c_str(),"recreate");

   if (sourcesfile.IsZombie()) {
      cerr << "TFile: Invalid file." << endl;
      exit(EXIT_FAILURE);
   }


   gDirectory->ls();

   // We need this to inform root we need dictionaries for std::complex.
   // This is a ROOT bug.
   gROOT->ProcessLine("#include <complex>");
   alm->Write();

   return 0;
}

