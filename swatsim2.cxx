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
#include "TFile.h"
#include <stdlib.h>

using namespace std;

template<typename T>
T mean(vector<T>& vec)
{
   // Mean of a vector, meant for double of float.

   if (vec.empty())
      throw "I do no want to calculate the mean of an empty vector.";

   typename vector<T>::iterator iter = vec.begin();
   T total = T(0);
   do {
      total += *++iter;
   } while (iter != vec.end());

   return total/vec.size();
}

template<typename T>
double stddev(vector<T>& vec, T mean)
{
   // Standard deviation, given a mean.

   if (vec.empty())
      throw "I do no want to calculate the mean of an empty vector.";

   typename vector<T>::iterator iter = vec.begin();
   T total = T(0);
   do {
      double tmp = *++iter - mean;
      total += tmp*tmp;
   } while (iter != vec.end());

   return sqrt(total/vec.size());
}

using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Looks for multiplets in simulated data. A simulated multiplet is read from\n\
   file provided in the command line, an isotropic sky is added and corralation\n\
   function is calculated for the events found. The wavelet transform is used\n\
   to find the Euler angles defining the position of the multiplet. The average\n\
   value of the correlation and number of events is reported.\n\
   WARNING: Three provided may be filled, make a backup.\n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-n nevents] \n\
           [-i emin] [-e emax] [-w width] [-l length] \n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     The scale, it is a number in the range 0 <= j <= 8, defaults to j = 1.\n\
   -n:     Number of events in the simulated sky, defaults to n = 1000.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -s:     Number of skies to simulate, defaults to 100.\n\
   -w:     Width of tangent plane used to calculate correlation, defaults to 2 degrees.\n\
   -l:     Length of tahgent plane used to calculate correlation, defaults to 10.\n\
   -f:     File from which multiplet will be read.\n\
   -N:     Band limit of wavelet to be used. Defaults to 1.\n" << endl;
}

int main(int argc,char* argv[])
{
   // Automates simulation for swat.

   int N = 1, j = 1, n = 1000, s = 100;
   double w = 3., width = 2., length = 10., min = 20., max = 40.;
   string emin = "20", emax = "40", file;

   char opt;

   while ((opt = getopt(argc,argv,"hj:N:n:i:e:w:l:s:f:")) != -1) {
      switch (opt) {
         case 'j':
	    j = atoi(optarg);
	    break;
         case 's':
	    s = atoi(optarg);
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
         case 'f':
	    file = optarg;
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
         case 'n':
	    n = atoi(optarg);
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   TF1 f("f","pow(x,-3)",min,max);

   TFile ff(file.c_str(),"update");

   if (ff.IsZombie()) {
      cerr << "Error: zombie file" << endl;
      exit(EXIT_FAILURE);
   }

   vector<double> corrs(s);
   vector<double> nevents(s);
   for (int sky = 0; sky < s; ++sky) {
      TRandom a(sky);
      gensky_from(n,a,&f);

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
      corrs[sky] = abs(g->GetCorrelationFactor());
      nevents[sky] = g->GetN();
      gDirectory->Clear();
   }

   double cmean = mean(corrs);
   double cstddev = stddev(corrs,cmean);
   double nmean = mean(nevents);
   double nstddev = stddev(nevents,nmean);

   cout << "C = " << cmean << " + " << cstddev << ", " << "n = " << nmean << " + " << nstddev << endl;
   return 0;
}


