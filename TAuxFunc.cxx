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
#include "TMath.h"
#include "TFile.h"
#include "TList.h"
#include "TKey.h"
#include "TClass.h"
#include "TString.h"
#include "TTree.h"
#include "TEventList.h"
#include "TRandom3.h"

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
      cout << "Ok, maximum error: (" << maxr << "," << maxi << ")" << endl;
      return true;
   } else {
      cout << "Not ok, maximum error: (" << maxr << "," << maxi << ")" << endl;
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

   try {
      TAnalysis analysis(sourcesfile,filename);
      analysis.SetLength(l);
      analysis.SetWidth(w);
      analysis.GenDeflectionGraphs();
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

   try {
      TAnalysis analysis(sourcesfile,filename);
      analysis.SetLength(l);
      analysis.SetWidth(w);
      analysis.CountEvents();
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
      gDirectory->Write();
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

   TSwatF forward(alm.GetJmax(),N,"M");
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

