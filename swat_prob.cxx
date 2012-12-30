#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>

#include "TFile.h"
#include "TH1D.h"
#include "TROOT.h"

#include <stdlib.h>

template <typename T>
T to_number(const char* str)
{
   std::istringstream s(str);
   T tmp;
   s >> tmp;
   return tmp;
}

using namespace std;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Reads a file output by swat_sim and calculates probabilities.\n\
   Usage: " << prog << " [-n n_steps] [-o outpu-file] [-t] [-k kind]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -o:     Output file, where probabilities will be recorded.\n\
   -t:     Calculates P(q >= q_0) if provided, P(q < q_0) otherwise.\n\
           q will be the correlation or wavelet coefficient, depending on \n\
	   the value passed in option -k\n\
   -k:     Either 1 for correlation or two for wavelet coefficient.\n\
   -n:     Total number of steps (points on probability graph). Defaults to 10.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   // Needed this to inform root we need dictionaries for std::complex.
   gROOT->ProcessLine("#include <TH1D.h>");

   int n_steps = 10;
   std::string output = "swat_prob_out.dat";
   bool type = false; // For option -t
   std::string which = "corr";     // For option -k

   char opt;

   while ((opt = getopt(argc,argv,"+hn:o:tk:")) != -1) {
      switch (opt) {
         case 'n': 
	    n_steps = to_number<int>(optarg);
	    break;
         case 'o': 
	    output = optarg;
	    break;
         case 't': 
	    type = true;
	    break;
         case 'k': {
	       int i = to_number<int>(optarg);
	       if (i == 1) {
	          which = "corr";
	       } else if (i == 2) {
	          which = "wav";
	       } else {
	          std::cerr << "Invalid argument for -k: It must be either 1 or 2." << std::endl;
	          exit(EXIT_FAILURE);
	       }
	    } break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   std::string input = argv[optind];
   TFile data(input.c_str());

   if (data.IsZombie()) {
      cerr << "TFile: Invalid input file." << endl;
      exit(EXIT_FAILURE);
   }

   TH1D* wav = (TH1D*) data.Get("wav");
   if (!wav) {
      std::cerr << "Invalid histogram wav in data file" << std::endl;
      exit(EXIT_FAILURE);
   }

   const int n_bins = wav->GetNbinsX();
   const double step = static_cast<double>(n_bins) / n_steps;

   // The number of simulations we can get from the histogram "wav".
   const int n_simulations = wav->GetEntries();

   TH1D* h = (TH1D*) data.Get(which.c_str());
   const int n_entries = h->GetEntries();
   const int diff = n_simulations - n_entries;

   std::ofstream out(output.c_str());
   out << "xcol ycol\n";

   if (type) {
      for (int i = 1; i < n_steps; ++i) {
	 double bin = step * i;
	 double total = h->Integral(bin, n_bins);
	 double p = (total + diff) / n_simulations;
	 double low_edge = h->GetBinLowEdge(bin);
	 out << low_edge << " " << (p * 100) << std::endl;
      }
   } else {
      for (int i = 1; i < n_steps; ++i) {
	 double bin = step * i;
	 double total = h->Integral(0, bin);
	 double p = (total + diff) / n_simulations;
	 double low_edge = h->GetBinLowEdge(bin);
	 out << low_edge << " " << (p * 100) << std::endl;
      }
   }

   return 0;
}

