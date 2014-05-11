#include <iostream>
#include <string>
#include <stdlib.h>

// ROOT

#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TEntryList.h"

#include <swat/TSourcesFinder.h>

static int B = 256;

using namespace std;

void print_usage(const char* prog)
{
   cout << "\n\n\
   Calculates the histogram of energy(field thirty nine), theta(field\n\
   seven) and phi (field six) in Herald data and save to file. The angular\n\
   variables are converted to radians before filling the histograms.\n\
   If no file is specified, an isotropic sky is generated.\n\n\
   Usage: " << prog << " [-i emin] [-e emax] [-f file.root]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -i:     Minimum energy of events, defaults to 20 EeV.\n\
   -e:     Maximum energy of events, defaults to 40 EeV.\n\
   -f:     Root file containing Tree with data, defaults to chain.root.\n\
   -n:     How many times histogram should be smoothed (TH1::Smooth operatin).\n\
   " << endl;
}

struct THists {
   TH1D* energy;
   TH2D* phi_theta;
   THists(): energy(0), phi_theta(0) {}
};

THists* hists_from_data(TFile& f,const std::string& cut, int ntimes)
{
   THists* hists = new THists;

   TTree* tree = (TTree*)f.Get("events");
   if (!tree) {
      cerr << "No TTree \"events\" in file: " << f.GetName() << endl;
      exit(EXIT_FAILURE);
   }

   // Lets generate a list with of desired events.
   tree->Draw(">>list",cut.c_str(),"entrylist");

   TEntryList* list = dynamic_cast<TEntryList*>(gDirectory->Get("list"));
   if (!list) {
      cerr << "Unable to retrieve list of events from root current directoy" << endl;
      exit(EXIT_FAILURE);
   }

   cout << "N = " << list->GetN() << " events passed selection." << endl;

   Double_t energy, theta, phi;
   tree->SetBranchAddress("thirtynine",&energy);
   tree->SetBranchAddress("seven",&theta);
   tree->SetBranchAddress("six",&phi);
   
   //create two histograms
   hists->energy   = new TH1D("energy","Energy distribution",100,4,3);
   hists->phi_theta = new TH2D("phi_theta","Phi and Theta distribution"
      , 2*B
      , 0
      , 2*TMath::Pi()
      , 2*B
      , TMath::Pi()/(4*B)
      , TMath::Pi() + 1./(4*B));
   
   // Read selected entries and fill the histograms.
   Long64_t nentries = 0;
   while (nentries < list->GetN()) {
     tree->GetEntry(list->Next());
     hists->energy->Fill(energy);
     hists->phi_theta->Fill(TMath::Pi()*(phi+180.)/180,TMath::Pi()*(theta+90.)/180);
     ++nentries;
  }

   int i = 0;
   while (++i < ntimes) 
      hists->phi_theta->Smooth(1);
  
   return hists;
}

struct THistsOwing {
   TH1D energy;
   TH2D phi_theta;
   THistsOwing()
   : energy("energy","Energy distribution",50,3,2)
   , phi_theta("phi_theta","Phi and Theta distribution."
      ,2*B
      ,0
      ,2*TMath::Pi()
      ,2*B
      ,TMath::Pi()/(4*B)
      ,TMath::Pi() + 1./(4*B)) { }
};


THistsOwing* isotropic_hists(const string& file,TF1& energy,int ntimes)
{
   THistsOwing* hists = new THistsOwing;
   TRandom3 r;  
   
   int i = 2001;
   double x,y,z;

   while (--i) {
      hists->energy.Fill(energy.GetRandom());
      r.Sphere(x,y,z,1);
      hists->phi_theta.Fill(TMath::ATan(y/x) + TMath::Pi()/2,TMath::ACos(z));
   }

   i = 2001;
   while (--i) {
      hists->energy.Fill(energy.GetRandom());
      r.Sphere(x,y,z,1);
      hists->phi_theta.Fill(TMath::ATan(y/x) + 3*TMath::Pi()/2,TMath::ACos(z));
   }

   i = 0;
   while (++i < ntimes) 
      hists->phi_theta.Smooth(1);

   return hists;
}

int main(int argc, char* argv[])
{
   string emin = "20", emax = "40", file = "chain.root", sources = "dist_hist_";
   bool isotropic = true;
   double min = 20, max = 40;
   int ntimes = 0; // How many times theta-phi histogram will be smoothed.

   char opt;
   while ((opt = getopt(argc,argv,"+hi:e:f:n:")) != -1) {
      switch (opt) {
         case 'i':
	    emin = optarg;
	    sources += "i";
	    sources += optarg;
	    min = atof(optarg);
	    break;
         case 'n':
	    sources += "n";
	    sources += optarg;
	    ntimes = atoi(optarg);
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
      THistsOwing* hists = isotropic_hists(file,energy,ntimes);
      TFile save_file(sources.c_str(),"recreate");
      hists->energy.Write();
      hists->phi_theta.Write();
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
      THists* hists = hists_from_data(f,cut,ntimes);
      TFile save_file(sources.c_str(),"recreate");
      hists->energy->Write();
      hists->phi_theta->Write();
      delete hists;
   }
}

