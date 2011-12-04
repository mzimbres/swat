#include <iostream>
#include <string>
#include <cmath>
#include "TRandom3.h"
#include "TF1.h"
#include "TAuxFunc.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"
#include "TGraph.h"
#include "TDirectory.h"
#include <stdlib.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Monte Carlo simulation. It calculates the probability\n\
   of a multiplet happen by chance on an isotropic sky.\n\
   The probability of a mltiplet happen by chance will be \n\
   the number of multiplet candidates passing crterium divided\n\
   by total number of skies simulated.\n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-n nevents] \n\
           [-i emin] [-e emax] [-w width] [-l length] \n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     The scale, it is a number in the range 0 <= j <= 8.\n\
           Defaults to j = 1.\n\
   -n:     Number of events in the simulated sky, defaults to\n\
           n = 1000\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -s:     Number of skies to simulate, defaults to 100.\n\
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
   string emin = "20", emax = "40";

   char opt;

   while ((opt = getopt(argc,argv,"hj:m:N:n:i:e:w:l:c:s:")) != -1) {
      switch (opt) {
         case 'j':
	    j = atoi(optarg);
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

   int tmp = 0;
   for (int sky = 0; sky < s; ++sky) {
      TRandom a(sky);
      gensky(n,a,&f);

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
	 cerr << "Graph has not been produced." << endl;
	 exit(EXIT_FAILURE);
      }
      double corr = abs(g->GetCorrelationFactor());
      double npoints = g->GetN();
      gDirectory->Clear();
      //cout << "C = " << corr << ", m = " << npoints << endl;
      if (corr >= C && npoints >= m)
         ++tmp;
   }

   cout << "P = " << (tmp*100./s) << "%" << endl;
   return 0;
}

