#include <iostream>
#include <string>
#include <stdlib.h>

// ROOT

#include "TTree.h"
#include "TH1D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TF1.h"

// SWAT

#include "TSourcesFinder.h"

using namespace std;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates the histogram of energy(field thirty nine), theta(field\n\
   seven) and phi (field six) in Herald data and save to file. The angular\n\
   variables are converted to radians before filling the histograms.\n\
   If no file is specified, an isotropic sky is generated.\n\
   If no herald file is specified, \n\
   Usage: " << prog << " [-i emin] [-e emax] [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   " << endl;
}

struct THists {
   TH1D* energy;
   TH1D* theta;
   TH1D* phi;
   THists(): energy(0), theta(0), phi(0) {}
};

struct THistsOwing {
   TH1D energy;
   TH1D theta;
   TH1D phi;
   THistsOwing()
   : energy("energy","Energy distribution",50,3,2)
   , theta("theta","Theta distribution",50,3,2)
   , phi("phi","Phi distribution",50,3,2) {}
};

THists* hists_from_data(TFile& f,const std::string& cut)
{
   THists* hists = new THists;

   TTree* tree = (TTree*)f.Get("events");
   if (!tree) {
      cerr << "No TTree \"events\" in file: " << f.GetName() << endl;
      exit(EXIT_FAILURE);
   }

   // Creates energy hist.
   Long64_t nevents = tree->Draw("thirtynine>>hist",cut.c_str());
   if (nevents == -1) {
      cerr << "Unable to scan the tree in: " << f.GetName() << endl;
      exit(EXIT_FAILURE);
   }

   cout << nevents << " events have passed the cut." << endl;

   // Creates theta hist.
   nevents = tree->Draw("(TMath::Pi()*(seven+90.)/180.)>>theta",cut.c_str());
   if (nevents == -1) {
      cerr << "Unable to scan the tree in: " << f.GetName() << endl;
      exit(EXIT_FAILURE);
   }

   // Creates phi hist.
   nevents = tree->Draw("(TMath::Pi()*(six+180.)/180)>>phi",cut.c_str());
   if (nevents == -1) {
      cerr << "Unable to scan the tree in: " << f.GetName() << endl;
      exit(EXIT_FAILURE);
   }

   hists->energy = (TH1D*)gDirectory->Get("hist");
   if (!hists->energy) {
      cerr << "Unable to retrieve hist from root directory" << endl;
      exit(EXIT_FAILURE);
   }
   hists->energy->SetNameTitle("energy","Energy distribution");

   hists->theta = (TH1D*)gDirectory->Get("theta");
   if (!hists->theta) {
      cerr << "Unable to retrieve hist from root directory" << endl;
      exit(EXIT_FAILURE);
   }
   hists->theta->SetNameTitle("theta","Theta distribution");

   hists->phi = (TH1D*)gDirectory->Get("phi");
   if (!hists->phi){
      cerr << "Unable to retrieve hist from root directory" << endl;
      exit(EXIT_FAILURE);
   }
   hists->phi->SetNameTitle("phi","Phi distribution");

   return hists;
}

THistsOwing* isotropic_hists(const string& file,TF1& energy)
{
   THistsOwing* hists = new THistsOwing;
   TRandom3 r;  
   
   int i = 1001;
   double x,y,z;

   while (--i) {
      hists->energy.Fill(energy.GetRandom());
      r.Sphere(x,y,z,1);
      hists->theta.Fill(TMath::ACos(z));
      hists->phi.Fill(TMath::ATan(y/x) + TMath::Pi()/2);
   }

   i = 1001;
   while (--i) {
      hists->energy.Fill(energy.GetRandom());
      r.Sphere(x,y,z,1);
      hists->theta.Fill(TMath::ACos(z));
      hists->phi.Fill(TMath::ATan(y/x) + 3*TMath::Pi()/2);
   }

   return hists;
}

int main(int argc, char* argv[])
{
   string emin = "20", emax = "40", file = "chain.root", sources = "energy_hist_";
   bool isotropic = true;
   double min = 20, max = 40;

   char opt;
   while ((opt = getopt(argc,argv,"+hi:e:f:")) != -1) {
      switch (opt) {
         case 'i':
	    emin = optarg;
	    sources += "i";
	    sources += optarg;
	    min = atof(optarg);
	    break;
         case 'e':
	    emax = optarg;
	    sources += "e";
	    sources += optarg;
	    max = atof(optarg);
	    break;
         case 'f':
	    file = optarg;
	    isotropic = false;
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   sources.append(".root");

   if (isotropic) {
      TF1 energy("f","pow(x,-3)",min,max);
      THistsOwing* hists = isotropic_hists(file,energy);
      TFile save_file(sources.c_str(),"recreate");
      hists->energy.Write();
      hists->theta.Write();
      hists->phi.Write();
      delete hists;
   } else {
      string cut = "(";
      cut.append(emin);
      cut.append(" < thirtynine && thirtynine < ");
      cut.append(emax);
      cut.append(" && ");
      cut.append(TSourcesFinder::fHeraldCut);
      cut.append(")");
      TFile f(file.c_str());
      THists* hists = hists_from_data(f,cut);
      TFile save_file(sources.c_str(),"recreate");
      hists->energy->Write();
      hists->theta->Write();
      hists->phi->Write();
      delete hists;
   }

}

