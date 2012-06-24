#include <iostream>
#include <string>
#include <stdlib.h>

// ROOT

#include "TTree.h"
#include "TH1D.h"

// SWAT

#include "TSourcesFinder.h"

using namespace std;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates the histogram of field in Herald file and save to file.\n\
   Usage: " << prog << " [-i emin] [-e emax] [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   " << endl;
}

int main(int argc, char* argv[])
{
   string emin = "20", emax = "40", file = "chain.root", sources = "e_hist";

   char opt;
   while ((opt = getopt(argc,argv,"+hi:e:f:")) != -1) {
      switch (opt) {
         case 'i':
	    emin = optarg;
	    sources += "i";
	    sources += optarg;
	    break;
         case 'e':
	    emax = optarg;
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

   sources.append(".root");

   TFile f(file.c_str());

   TTree* tree = (TTree*)f.Get("events");
   if (!tree) {
      cerr << "No TTree \"events\" in file: " << file << endl;
      exit(EXIT_FAILURE);
   }

   string cut = "(";
   cut.append(emin);
   cut.append(" < thirtynine && thirtynine < ");
   cut.append(emax);
   cut.append(" && ");
   cut.append(TSourcesFinder::fHeraldCut);
   cut.append(")");

   Long64_t nevents = tree->Draw("thirtynine>>hist",cut.c_str());
   if (nevents == -1) {
      cerr << "Unable to scan the tree in: " << file << endl;
      exit(EXIT_FAILURE);
   }

   cout << nevents << " events have passed the cut." << endl;

   //gDirectory->ls();
   TH1D *hist = (TH1D*)gDirectory->Get("hist");
   if (!hist) {
      cerr << "Unable to retrieve hist from root directory" << endl;
      exit(EXIT_FAILURE);
   }

   hist->SetNameTitle("energy","Energy distribution");
   TFile save_file(sources.c_str(),"recreate");
   hist->Write();
}

