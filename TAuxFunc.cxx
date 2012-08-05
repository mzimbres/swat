// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 2011 Marcelo Zimbres Silva
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <memory>
#include <string>
#include <complex>
#include <fstream>

// ROOT

#include "TObject.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TFile.h"
#include "TList.h"
#include "TKey.h"
#include "TClass.h"
#include "TString.h"
#include "TTree.h"
#include "TEventList.h"
#include "TRandom3.h"
#include "TF1.h"

// SWAT

#include "TAuxFunc.h"
#include "TWavelet.h"
#include "TAlm.h"
#include "TSkyMap.h"
#include "TWavMap.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"
#include "TSwatF.h"
#include "THealpixMap.h" 
#include "TSphHarmF.h" 
#include "TSwatB.h" 

// Healpix

#include "Healpix_Map.h"
#include "arr.h"
#include "alm_map_tools.h"
#include "Alm.h"
#include "pointing.h"
#include "xcomplex.h"
#include "alm_healpix_tools.h"


   ////////////////////////////////////////////////////////////////
   //
   //  TAuxFunc
   //
   //  Some useful functions.
   //
   //
   ///////////////////////////////////////////////////////////////

using namespace TMath;
using namespace std;

//___________________________________________________________________
TAlm* TAuxFunc::alm2wav2alm(const TAlm& alm,Int_t N,Int_t jmin,Int_t jmax)
{
   // Transform alm to wavelet domain, using wavelet with azimuthal
   // band limit N > 0. The filtered coefficients are returned
   // and user is responsible for deleting it.
   // The returned alm will have average set to zero(alm(0,0) = 0)
   
   Int_t J = alm.GetJmax();
   if (jmax == -1) 
      jmax = J;

   if (jmin > jmax || jmax > J || jmin < 0) {
      cout << "Incompatible values of jmin, jmax and J." << endl;
      return NULL;
   }

   TAlm* alm2 = new TAlm(J);

   for(Int_t i=jmin;i<=jmax;++i){
      TWavMap* wav = TAuxFunc::SWAT(alm,i,N);
      TAlm tmp(alm.GetJmax());
      wav->CreateAlm(tmp);
      delete wav;
      alm2->Add(tmp);
   }

   return alm2;
}

//___________________________________________________________________
TAlm* TAuxFunc::rand_gaus_alm(Int_t J,Double_t mean,Double_t sigma)
{
   // Random alm

   TAlm* alm = new TAlm(J);

   TRandom3 r;
   for(size_t l = 1;l < alm->GetL();++l)
      for(size_t m = 1;m <= l;++m)
         (*alm)(l,m) = complex<Double_t>(r.Gaus(mean,sigma),r.Gaus(mean,sigma));
      

   for(size_t l = 1;l < alm->GetL();++l) // No imaginary part for m = 0.
         (*alm)(l,0) = complex<Double_t>(r.Gaus(mean,sigma),0);

   return alm;
}

//___________________________________________________________________
bool TAuxFunc::compare_alm(const TAlm& alm1,const TAlm& alm2)
{
   // Compare two alm's printing the maximum of the 
   // difference diff = alm1(l,m) - alm2(l,m). Returns true
   // if difference is less than 10e-10

   if (alm1.GetJmax() != alm2.GetJmax()){
      cout << "TAlm objects of different sizes." << endl;
      return false;
   }
       
   Double_t rei,imi,reo,imo,maxr = 0,maxi = 0;

   for(size_t l = 1;l < alm1.GetL(); ++l){
      for(size_t m = 0;m <= l; ++m){
	 rei = real(alm1(l,m));
	 reo = real(alm2(l,m));
	 imi = imag(alm1(l,m));
	 imo = imag(alm2(l,m));
         Double_t diff1 = rei - reo;
         Double_t diff2 = imi - imo;
         Double_t temp1 = Max(Abs(diff1),maxr);
         Double_t temp2 = Max(Abs(diff2),maxi);
	 if (temp1 > maxr) maxr = temp1;
	 if (temp2 > maxi) maxi = temp2;
      }
   }

   if ((maxr <= 10e-10) && (maxi <= 10e-10)) {
      //cout << "Ok, maximum error: (" << maxr << "," << maxi << ")" << endl;
      return true;
   } else {
      cerr << "Not ok, maximum error: (" << maxr << "," << maxi << ")" << endl;
   }

   return false;
}

void TAuxFunc::find_multiplets( Double_t l, Double_t w, 
                                const char* sourcesfile, 
				const char* filename)
{
   // This function will calculate deflections versus 1/E graphs and and to the
   // sources file. The same will be done to the graphs of number of events versus 
   // orientation.

   TFile sources(sourcesfile,"update");

   if (sources.IsZombie()) {
      cerr << "TAuxFunc::find_multiplets(): File with sources not found\n";
      return;
   }

   TFile data(filename);

   if (data.IsZombie()) {
      cerr << "TAuxFunc::find_multiplets(): File with data not found\n";
      return;
   }

   try {
      TAnalysis analysis(&sources,&data);
      analysis.SetLength(l);
      analysis.SetWidth(w);
      analysis.GenDeflectionGraphs();
      sources.Write(0,TObject::kOverwrite);
      sources.Close();
   } catch (const char* str) {
      cout << str << endl;
      terminate();
   }
}

void TAuxFunc::n_vs_gamma( Double_t l, Double_t w, 
                                const char* sourcesfile, 
				const char* filename)
{
   // This function will calculate deflections versus 1/E graphs and and to the
   // sources file. The same will be done to the graphs of number of events versus 
   // orientation.

   TFile sources(sourcesfile,"update");

   if (sources.IsZombie()) {
      cerr << "TAuxFunc::n_vs_gamma(): File with sources not found\n";
      return;
   }

   TFile data(filename);

   if (data.IsZombie()) {
      cerr << "TAuxFunc::n_vs_gamma(): File with data not found\n";
      return;
   }

   try {
      TAnalysis analysis(&sources,&data);
      analysis.SetLength(l);
      analysis.SetWidth(w);
      analysis.CountEvents();
      sources.Write(0,TObject::kOverwrite);
      sources.Close();
   } catch (const char* str) {
      cout << str << endl;
      terminate();
   }
}

void TAuxFunc::find_sources(const char* emin,
                            const char* emax,
                            Int_t N,
			    Int_t j,
			    Int_t nsources, 
			    Double_t w,
			    const char* filename)
{
   // Read herald file in the current directory, uses wavelets to find
   // sources and save to root file together with TEventList. file name
   // will be sources.root
   // cuts: cuts to be applied. Herald suggested cuts will be added.
   // j: Scale for the wavelet transform
   // nsource: Number of sources 
   // w: Sources whose distance are less than w, will be considered 
   // the same


   // Gets pointer to the TTree object in the current directory
   // generate an event list for the cut provided and sets the 
   // cut on the TTree.

   std::auto_ptr<TFile> fEventsFile(TFile::Open(filename));

   if (fEventsFile->IsZombie()) {
      std::cerr << "File "<< filename <<" not found.\n";
      return;
   }

   try {
      TSourcesFinder finder;
      finder.SetMinEnergy(emin);
      finder.SetMaxEnergy(emax);
      finder.SetN(N);
      finder.SetScale(j);
      finder.SetNSources(nsources);
      finder.SetSeparation(w);
      TFile f("sources.root","recreate");
      std::cout << "Finding sources ...\n";
      finder.FindSources();
      gDirectory->ls();
      gDirectory->Write(0,TObject::kOverwrite);
      f.Close();
   } catch(const char* message) {
      cout << "Exception caught: " <<  message << endl;
      return;
   }
}

//________________________________________________________
TWavMap* TAuxFunc::SWAT(const TAlm& alm,int j,int N)
{
   // Transform alm to wavelet domain, using wavelet with azimuthal
   // band limit N > 0 at scale j.
   // Only cylindrical pixelization is provided.

   TSwatF forward(alm.GetJmax(),N,"ES");
   forward.SetPoints(alm,j);
   forward.Transform();

   return forward.GetPoints();
}

//________________________________________________________
TVMap* TAuxFunc::SHT(const TAlm& alm,bool healpixmap)
{
   // Performs a spherical harmonic transform. If healpixmap is true
   // create a THealpixMap, otherwise a TSkyMap.

   size_t L = alm.GetL();
   int J = alm.GetJmax();

   if (healpixmap) {

      Alm<xcomplex<double> > alm1(L-1,L-1);

      xcomplex<double> c;
      for (size_t l = 0; l < L; ++l){
	 for (size_t m = 0; m <= l; ++m){
	    c.real() = real(alm(l,m));
	    c.imag() = imag(alm(l,m));
	    alm1(l,m) = c;
	 }
      }

      Healpix_Map<double> map(L,RING,SET_NSIDE);
      alm2map<double>(alm1,map);

      THealpixMap* mapa = new THealpixMap(J);
      mapa->Set(mapa->GetNPix(),&map[0]);

      return mapa;
   } else {
      TSphHarmF forward(J);
      forward.SetPoints(alm);
      forward.Transform();
      return forward.GetPoints();
   }

}

TTree* TAuxFunc::gensky(int n,TH1D* energy,TH2D* phi_theta)
{
   // Generate a TTree with name events and adds it to the current directory.
   // The tree contains a CRPropa-like tree, that can be used in other
   // analysis code. Algorithm sets only theta phi and energy, other branch
   // are filled with not meaningfull values.

   TTree* newt = new TTree("events","CRPropa 3D events");
   float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;

   newt->Branch("Particle_Type"         ,&y1 , "Particle_Type/F"         ); 
   newt->Branch("Initial_Type"          ,&y2 , "Initial_Type/F"          );
   newt->Branch("Initial_Position_X_Mpc",&y3 , "Initial_Position_X_Mpc/F");
   newt->Branch("Initial_Position_Y_Mpc",&y4 , "Initial_Position_Y_Mpc/F");
   newt->Branch("Initial_Position_Z_Mpc",&y5 , "Initial_Position_Z_Mpc/F");
   newt->Branch("Initial_Momentum_E_EeV",&y6 , "Initial_Momentum_E_EeV/F");
   newt->Branch("Initial_Momentum_theta",&y7 , "Initial_Momentum_theta/F");
   newt->Branch("Initial_Momentum_phi"  ,&y8 , "Initial_Momentum_phi/F"  );
   newt->Branch("Time_Mpc"              ,&y9 , "Time_Mpc/F"              );
   newt->Branch("Position_X_Mpc"        ,&y10,"Position_X_Mpc/F"        );
   newt->Branch("Position_Y_Mpc"        ,&y11,"Position_Y_Mpc/F"        );
   newt->Branch("Position_Z_Mpc"        ,&y12,"Position_Z_Mpc/F"        );
   newt->Branch("Momentum_E_EeV"        ,&y13,"Momentum_E_EeV/F"        );
   newt->Branch("Momentum_theta"        ,&y14,"Momentum_theta/F"        );
   newt->Branch("Momentum_phi"          ,&y15,"Momentum_phi/F"          );

   double theta,phi;
   for (int i = 0; i < n; ++i) {
      y1  = 1;   
      y2  = 1; 
      y3  = 1; 
      y4  = 1; 
      y5  = 1; 
      y6  = 1; 
      y7  = 1; 
      y8  = 1; 
      y9  = 1; 
      y10 = 10; 
      y11 = 11; 
      y12 = 12; 
      y13 = energy->GetRandom(); 
      phi_theta->GetRandom2(phi,theta);
      y14 = theta;
      y15 = phi;
      newt->Fill();
   }
   return newt;
}

void TAuxFunc::gensky_from(int n,TH1D* energy,TH2D* phi_theta)
{
   // Adds events from TTree called events in file.
   // The tree contains a CRPropa-like tree, that can be used in other
   // analysis code. Algorithm sets only theta phi and energy, other branch
   // are filled with not meaningfull values.

   TTree* newt = (TTree*) gDirectory->Get("events");

   if (!newt) {
      cerr << "Not TTree events in directory." << endl;
      return;
   }
   float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;

   TBranch        *b_y1; 
   TBranch        *b_y2;
   TBranch        *b_y3;
   TBranch        *b_y4;
   TBranch        *b_y5;
   TBranch        *b_y6;
   TBranch        *b_y7;
   TBranch        *b_y8;
   TBranch        *b_y9;
   TBranch        *b_y10;
   TBranch        *b_y11;
   TBranch        *b_y12;
   TBranch        *b_y13;
   TBranch        *b_y14;
   TBranch        *b_y15;

   newt->SetBranchAddress("Particle_Type"         ,&y1 ,&b_y1 ); 
   newt->SetBranchAddress("Initial_Type"          ,&y2 ,&b_y2 );
   newt->SetBranchAddress("Initial_Position_X_Mpc",&y3 ,&b_y3 );
   newt->SetBranchAddress("Initial_Position_Y_Mpc",&y4 ,&b_y4 );
   newt->SetBranchAddress("Initial_Position_Z_Mpc",&y5 ,&b_y5 );
   newt->SetBranchAddress("Initial_Momentum_E_EeV",&y6 ,&b_y6 );
   newt->SetBranchAddress("Initial_Momentum_theta",&y7 ,&b_y7 );
   newt->SetBranchAddress("Initial_Momentum_phi"  ,&y8 ,&b_y8 );
   newt->SetBranchAddress("Time_Mpc"              ,&y9 ,&b_y9 );
   newt->SetBranchAddress("Position_X_Mpc"        ,&y10,&b_y10);
   newt->SetBranchAddress("Position_Y_Mpc"        ,&y11,&b_y11);
   newt->SetBranchAddress("Position_Z_Mpc"        ,&y12,&b_y12);
   newt->SetBranchAddress("Momentum_E_EeV"        ,&y13,&b_y13);
   newt->SetBranchAddress("Momentum_theta"        ,&y14,&b_y14);
   newt->SetBranchAddress("Momentum_phi"          ,&y15,&b_y15);

   double theta,phi;
   for (int i = 0; i < n; ++i) {
      y1  = 1;   
      y2  = 1; 
      y3  = 1; 
      y4  = 1; 
      y5  = 1; 
      y6  = 1; 
      y7  = 1; 
      y8  = 1; 
      y9  = 1; 
      y10 = 10; 
      y11 = 11; 
      y12 = 12; 
      y13 = energy->GetRandom(); 
      phi_theta->GetRandom2(phi,theta);
      y14 = theta;
      y15 = phi;
      newt->Fill();
   }
}

TAlm* TAuxFunc::wav2alm(const TWavMap& wav)
{
   // Uses Spherical WAvelet Transform to do a backward transformation
   // to harmonic space(TAlm).

   TAlm* alm = new TAlm(wav.GetJ());
   TSwatB backward(wav.GetJ(),wav.GetN(),"M");
   backward.SetPoints(wav);
   backward.Transform();
   backward.GetAlm(*alm,wav.GetScale());
   return alm;
}

