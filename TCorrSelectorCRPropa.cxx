#define TCorrSelectorCRPropa_cxx

#include <iostream>
#include <sstream>
#include <cmath>

#include <TH2.h>
#include <TStyle.h>
#include <TDirectory.h>

#include "TMath.h"
#include "TCanvas.h"
#include "THistPainter.h"
 
#include "TCorrSelectorCRPropa.h"
#include "TEulerAngle.h"

using namespace TMath;
using namespace std;

void TCorrSelectorCRPropa::Begin(TTree * /*tree*/)
{
   J = 8;
   fCount = 0;

   fEnergy.resize(0);
   fAngle2.resize(0);

   TString filename("corr_graph");
   filename += fCountCalls;
   filename += ".dat";

   corr.open(filename.Data());
   if (!corr.good()){
      cerr << "Failed to open file: " << filename.Data() << endl;
      return;
   }

   corr  << "xcolname ycolname colordata " << fCountCalls << "\n";

   ++fCountCalls;

   out.open("mult_cand.dat",ios_base::app);
   if (!out.good()){
      cerr << "Failed to open file: mult_cand.dat" << endl;
      return;
   }

   out << "%xcolname ycolname colordata " << fCountCalls << "\n";

   TString option = GetOption();
}

void TCorrSelectorCRPropa::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
}

Bool_t TCorrSelectorCRPropa::Process(Long64_t entry)
{
   b_Momentum_theta->GetEntry(entry);
   b_Momentum_phi->GetEntry(entry);
   b_Momentum_E_EeV->GetEntry(entry);

   Double_t theta = fmod(static_cast<Double_t>(Momentum_theta) , Pi());
   Double_t phi   = fmod((static_cast<Double_t>(Momentum_phi) + 2*Pi()) , 2*Pi());

   Double_t six = phi*180./Pi() - 180.;
   Double_t seven = theta*180./Pi() - 90.;

   if (fAngle->IsInsideRec(theta,phi,fAngSep)) {
       Double_t oneover = 1/Momentum_E_EeV;
       fEnergy.push_back(oneover); 
       Double_t ang = -fAngSep*180./Pi();
       fAngle2.push_back(ang); 
       ++fCount;
       corr  << oneover  << " " << ang << " " << Momentum_E_EeV << "\n";

       Double_t x, y;
       THistPainter::ProjectAitoff2xy(six,seven,x,y);
       out << x << " " << y << " " << Momentum_E_EeV << "\n";
   }

   return kTRUE;
}

void TCorrSelectorCRPropa::SlaveTerminate()
{

}

void TCorrSelectorCRPropa::Terminate()
{
   corr.close();
   out.close();
   delete fAngle;

   //Int_t size = fEnergy.size();

   //if (size == 0) {
   //   cout << "No points selected." << endl;
   //   return;
   //}

   fGraph = new TGraph(fEnergy.size(),&fEnergy[0],&fAngle2[0]);

   fGraph->GetXaxis()->SetTitle("1/E [EeV^{-1}]");
   fGraph->GetYaxis()->SetTitle("Deflection Angle [degrees]");

   ostringstream oss;
   oss.precision(3);
   oss << fixed << fGraph->GetCorrelationFactor();
   string title = "C = " + oss.str() + ", N = ";
   ostringstream oss1;
   oss1 << fCount;
   title += oss1.str();

   fGraph->SetNameTitle(fGraphName.Data(),title.c_str());
   //fGraph->Draw("AP");
   gDirectory->Add(fGraph,kTRUE);
}

void TCorrSelectorCRPropa::SetEulerAngles(const TEulerAngle* ang)
{
   fAngle = new TAngDistance(*ang);
}

void TCorrSelectorCRPropa::ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c)
{
   fAngle->ShiftAlphaBetaGamma(a,b,c);
}

void TCorrSelectorCRPropa::SetGraphName(const TString& str)
{
   fGraphName = str;
}

void TCorrSelectorCRPropa::SetLengthWidth(Double_t l,Double_t w)
{
   fAngle->SetLength(l);
   fAngle->SetWidth(w);
}
