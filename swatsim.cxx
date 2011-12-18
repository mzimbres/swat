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
#include "TH1D.h"
#include <stdlib.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates the probability of a multiplet happen by chance on an\n\
   isotropic sky. The probability of a multiplet happen by chance will be\n\
   the number of multiplet candidates passing criterium divided by total\n\
   number of skies simulated. It also calculates the histogram of the\n\
   correlation function and saves it the root file correlation.root.\n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-n nevents] \n\
           [-i emin] [-e emax] [-w width] [-l length] [-f file.root]\n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     The scale, it is a number in the range 0 <= j <= 8.\n\
           Defaults to j = 1.\n\
   -n:     Number of events in the simulated sky, defaults to\n\
           n = 1000\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -s:     Number of skies to simulate, defaults to 100.\n\
   -f:     Add events in TTree stored in file.\n\
   -c:     Minimum correlation, defaults to 0.2\n\
   -m:     Minimum number of events hitting tangent plane, that will be used\n\
           to calculate the correlation, defaults to 4. If less than m, the sky\n\
	   will not pass criterium.\n\
   -w:     Width of tahgent plane used to calculate correlation,\n\
           defaults to 2 degree.\n\
   -l:     Length of tahgent plane used to calculate correlation,\n\
           defaults to 10.\n\
   -N:     Band limit of wavelet to be used. Defaults to 1.\n" << endl;
}

int main(int argc,char* argv[])
{
   // Automates simulation for swat.

   int N = 1, j = 1, n = 1000, m = 4, s = 100;
   double w = 3., width = 2., length = 10., min = 20., max = 40., C = 0.2;
   string emin = "20", emax = "40", file;
   bool add = false;

   char opt;

   while ((opt = getopt(argc,argv,"+hj:m:N:n:i:e:w:l:c:s:f:")) != -1) {
      switch (opt) {
         case 'j': j = atoi(optarg);
	    break;
         case 's':
	    s = atoi(optarg);
	    break;
         case 'm':
	    m = atoi(optarg);
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
	    add = true;
	    break;
         case 'w':
	    width = atof(optarg);
	    break;
         case 'l':
	    length = atof(optarg);
	    break;
         case 'c':
	    C = atof(optarg);
	    break;
         case 'N':
	    if ((N = atoi(optarg)) < 1) {
	       cerr << "N = " << N << ", N must be greaer than 0" << endl;
	       exit(EXIT_FAILURE);
	    }
	    break;
         case 'n':
	    n = atoi(optarg);
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   TF1 f("f","pow(x,-3)",min,max);
   TH1D hist("corr","Correlation",50,0,1);
   TH1D hist2("n","Number of events hitting plane",12,0,12);

   TTree* addtree = 0;
   if (add) {
      TFile addfile(file.c_str());

      if (addfile.IsZombie()) {
         cerr << "TFile: Invalid file." << endl;
	 exit(EXIT_FAILURE);
      }

      addtree = (TTree*) addfile.Get("events");
      if (addtree == 0) {
         cerr << "TFile: No Tree events in file." << endl;
	 exit(EXIT_FAILURE);
      }
   }

   int tmp = 0;
   for (int sky = 0; sky < s; ++sky) {
      TFile temp("temp.root","recreate");
      TRandom a(sky);
      TTree* t = gensky(n,a,&f);

      gDirectory->Add(t);

      if (add) {
	 TList list(addtree);
	 t->Merge(&list);
      }

      TSourcesFinder finder;
      finder.SetMinEnergy(emin.c_str());
      finder.SetMaxEnergy(emax.c_str());
      finder.SetN(N);
      finder.SetScale(j);
      finder.SetNSources(1);
      finder.SetSeparation(w);
      finder.FindSources();

      TAnalysis analysis(gDirectory,gDirectory);
      analysis.SetLength(length/2);
      analysis.SetWidth(width/2);
      analysis.GenDeflectionGraphs();
      TGraph* g = (TGraph*)gDirectory->Get("g0");
      if (!g) {
         hist2.Fill(0);
         gDirectory->Clear();
         continue;
      }

      double corr = abs(g->GetCorrelationFactor());
      double npoints = g->GetN();
      hist2.Fill(npoints);
      gDirectory->DeleteAll();
      if (npoints >= m) 
         hist.Fill(corr);

      if (corr >= C && npoints >= m)
         ++tmp;

   }
   TFile fff("correlation.root","recreate");
   hist.Write();
   hist2.Write();
   fff.Close();

   cout << "P = " << (tmp*100./s) << "%" << endl;
   return 0;
}

