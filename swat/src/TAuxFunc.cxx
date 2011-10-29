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
#include "TVMap.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"

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
bool TAuxFunc::valid_file(const TFile& file,const char* classname)
{
   // Check if all objects in file are of type classname.

   Int_t nobjs = file.GetNkeys();

   TListIter iter(file.GetListOfKeys());

   Int_t count = 0;
   while(TKey* key = (TKey*)iter.Next()){
      TClass tmp(key->GetClassName());
      if (tmp.InheritsFrom(classname))
         ++count;
   }

   if (count != nobjs){
      cout << (nobjs - count) << " objects in file " << file.GetName();
      cout << " are not of type " << classname << endl;
      return kFALSE;
   }

   return kTRUE;
}

//___________________________________________________________________
TObject* TAuxFunc::retrieve(TFile* f,const char* classname)
{
   // Brings object of class classname on file into memory and returns 
   // a pointer to it. If object is not a classname object, 
   // returns a null pointer.

   TObject *obj = NULL;
   if (!TAuxFunc::valid_file(*f,classname) || f->IsZombie())
      return obj;
   
   TList *list = f->GetListOfKeys();
   TKey *key = (TKey*)list->First();
   TString cl(key->GetClassName());
   obj = f->Get(key->GetName());
   f->Close();

   return obj;
}

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
      TVMap* wav = alm.SWAT(i,N);
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
   for(Int_t l = 1;l < alm->GetL();++l)
      for(Int_t m = 1;m <= l;++m)
         (*alm)(l,m) = complex<Double_t>(r.Gaus(mean,sigma),r.Gaus(mean,sigma));
      

   for(Int_t l = 1;l < alm->GetL();++l) // No imaginary part for m = 0.
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

   for(Int_t l = 1;l < alm1.GetL(); ++l){
      for(Int_t m = 0;m <= l; ++m){
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

   try {
      TSourcesFinder finder(filename);
      finder.SetMinEnergy(emin);
      finder.SetMaxEnergy(emax);
      finder.SetN(N);
      finder.SetScale(j);
      finder.SetNSources(nsources);
      finder.SetSeparation(w);
      finder.FindSources();
   } catch(const char* message) {
      cout << "No able to find sources, Exception message: "<< endl;
      cout << message << endl;
      return;
   }
}

