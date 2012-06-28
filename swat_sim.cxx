#include <iostream>
#include <string>
#include <cmath>
#include <stdlib.h>

// ROOT

#include "TRandom3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// SWAT

#include "TAuxFunc.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates the probability of a multiplet with minimum correlation C (see -c\n\
   option) and minimum number of events m (see -m option) happen by chance\n\
   using wavelet analysis. First an isotropic sky is simulated and the wavelet\n\
   representation of the sky is calculated, the euler angles of the largest\n\
   coefficient is used to calculate the equations of the tangent plane at the\n\
   position found (the euler angles). The correlation C is calculated including\n\
   all events that hit the tangent plane, whose size is specified with the\n\
   options -l and -w. The probablility will be the number of multiplets with\n\
   correlation > C and number of events > m, divided by the number of skies\n\
   simulated. Additionaly, three other quantities are calculated, the histogram\n\
   of the number of events that hit the tangent plane, the histogram of the\n\
   C's found for which the number of events is greater than m (passed in the\n\
   command line) and the histogram of the magnitude of wavelet coefficients.\n\
   If -f option is used, a TTree in the file will be read and events will be\n\
   added to the analysis, this is useful to include a simulated multiplet on\n\
   the analysis, hiding it in the isotropic backgroung the test the algorithm.\n\
   \n\
   It is also mandatory to specify:\n\
   \n\
      - Energy distribution.\n\
      - The theta distribution.\n\
      - The phi distribution.\n\
   \n\
   This is the distribution the background events have to follow. These\n\
   distributions are read from a root file. Use swat_gen to generate them.\n\
   \n\n\
   Usage: " << prog << " [ -j scale] [-N number] [-n nevents] [-s skies]\n\
           [-i emin] [-e emax] [-c corr] [-m mevents] [-w width] [-l length]\n\
	   [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -j:     Wavelet scale a number in the range 0 <= j <= 8, defaults to 1.\n\
   -N:     Band limit of wavelet, in the range 0 < N <= 128, defaults to 1.\n\
   -n:     Number of events in the simulated sky, defaults to n = 1000\n\
   -s:     Number of skies to simulate, defaults to 100.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -c:     Minimum correlation, defaults to 0.2.\n\
   -m:     Minimum number of events hitting tangent plane.\n\
   -w:     Width of tangent plane, defaults to 2 degrees.\n\
   -l:     Length of tangent plane, defaults to 10 degrees.\n\
   -f:     Add events in TTree stored in file to the simulated sky.\n\
   -d:     Histogram with energy, theta and phi distributions.\n\
   " << endl;
}

int main(int argc,char* argv[])
{
   // Automates simulation for swat.

   int N = 1, j = 1, n = 1000, m = 4, s = 100;
   double w = 3., width = 2., length = 10., min = 20., max = 40., C = 0.2;
   string emin = "20", emax = "40", file, outfilename = "hist_", dist_file;
   bool add = false;
   TH1D* energy = 0;
   TH2D* phi_theta = 0;

   char opt;

   while ((opt = getopt(argc,argv,"+hj:m:N:n:i:e:w:l:c:s:f:d:")) != -1) {
      switch (opt) {
         case 'j': 
	    j = atoi(optarg);
	    outfilename += "j";
	    outfilename += optarg;
	    break;
         case 's':
	    outfilename += "s";
	    outfilename += optarg;
	    s = atoi(optarg);
	    break;
         case 'm':
	    outfilename += "m";
	    outfilename += optarg;
	    m = atoi(optarg);
	    break;
         case 'i':
	    emin = optarg;
	    outfilename += "i";
	    outfilename += optarg;
	    min = atof(optarg);
	    break;
         case 'e':
	    emax = optarg;
	    max = atof(optarg);
	    outfilename += "e";
	    outfilename += optarg;
	    break;
         case 'f':
	    file = optarg;
	    outfilename += "with";
	    add = true;
	    break;
         case 'w':
	    width = atof(optarg);
	    break;
         case 'l':
	    length = atof(optarg);
	    outfilename += "l";
	    outfilename += optarg;
	    break;
         case 'c':
	    C = atof(optarg);
	    break;
         case 'N':
	    if ((N = atoi(optarg)) < 1) {
	       cerr << "N = " << N << ", N must be greaer than 0" << endl;
	       exit(EXIT_FAILURE);
	    }
	    outfilename += "N";
	    outfilename += optarg;
	    break;
         case 'n':
	    n = atoi(optarg);
	    outfilename += "n";
	    outfilename += optarg;
	    break;
         case 'd':
	    dist_file = optarg;
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   if (dist_file.empty()) {
      cerr << "\n\
You have to specify a file with distributions of\n\
energy, theta and phi that will be used to generate the \n\
background. See option -d\n" << endl;
      exit(EXIT_FAILURE);
   }

   TFile distributions(dist_file.c_str());
   if (distributions.IsZombie()) {
      cerr << "TFile: Invalid distribution file." << endl;
      exit(EXIT_FAILURE);
   }

   energy = (TH1D*)distributions.Get("energy");
   if (!energy) {
      cerr << "Unable to read energy histogram from file." << endl;
      exit(EXIT_FAILURE);
   }

   phi_theta = (TH2D*)distributions.Get("theta");
   if (!phi_theta) {
      cerr << "Unable to read theta histogram from file." << endl;
      exit(EXIT_FAILURE);
   }

   TH1D hist("corr","Correlation",50,0,1);
   TH1D hist2("n","Number of events hitting plane",35,0,35);
   TH1D wav_mag("wav","Magnitude of wavelet coefficient",50,0.04,0.03);

   int tmp = 0;
   if (add) {
      TFile addfile(file.c_str());

      if (addfile.IsZombie()) {
	 cerr << "TFile: Invalid file." << endl;
	 exit(EXIT_FAILURE);
      }

      TTree* addtree = (TTree*) addfile.Get("events");
      if (addtree == 0) {
	 cerr << "TFile: No Tree events in file." << endl;
	 exit(EXIT_FAILURE);
      }
      addtree->SetBranchStatus("*",1);

      for (int sky = 0; sky < s; ++sky) {
	 TFile ff("removeme.root","recreate");
	 TTree* t = addtree->CloneTree();
	 TRandom a(sky);
	 gDirectory->Add(t);
	 gensky_from(n,energy,phi_theta);
	 
	 TSourcesFinder finder;
	 finder.SetMinEnergy(emin.c_str());
	 finder.SetMaxEnergy(emax.c_str());
	 finder.SetN(N);
	 finder.SetScale(j);
	 finder.SetNSources(1);
	 finder.SetSeparation(w);
	 wav_mag.Fill(finder.FindSources());

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
	 if (npoints >= m) hist.Fill(corr);

	 if (corr >= C && npoints >= m) ++tmp;
      }
   } else {
      for (int sky = 0; sky < s; ++sky) {
	 TFile ff("temp.root","recreate");
	 TRandom a(sky);
	 TTree* t = gensky(n,energy,phi_theta);
	 gDirectory->Add(t);

	 TSourcesFinder finder;
	 finder.SetMinEnergy(emin.c_str());
	 finder.SetMaxEnergy(emax.c_str());
	 finder.SetN(N);
	 finder.SetScale(j);
	 finder.SetNSources(1);
	 finder.SetSeparation(w);
	 wav_mag.Fill(finder.FindSources());

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
	 if (npoints >= m) hist.Fill(corr);

	 if (corr >= C && npoints >= m) ++tmp;
      }
   }
   if (outfilename.empty()) outfilename = "outfile";
   outfilename += ".root";
   TFile fff(outfilename.c_str(),"recreate");
   hist.Write();
   hist2.Write();
   wav_mag.Write();
   fff.Close();

   cout << "P = " << (tmp*100./s) << "%" << endl;
   return 0;
}

