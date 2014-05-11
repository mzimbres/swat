// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2011 Marcelo Zimbres Silva
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

#include <complex>

// ROOT

#include "TArrayD.h"
#include "TH2D.h"
#include "TF1.h"
#include "TObject.h"
#include "TMath.h"

// SWAT

#include <swat/swat/swat.h>
#include <swat/swat/THealpixMap.h> 
#include <swat/swat/TAlm.h>

// Healpix

#include <swat/healpix/Healpix_Map.h>
#include <swat/healpix/arr.h>
#include <swat/healpix/alm_map_tools.h>
#include <swat/healpix/Alm.h>
#include <swat/healpix/pointing.h>
#include <swat/healpix/xcomplex.h>
#include <swat/healpix/alm_healpix_tools.h>

using namespace TMath;
using namespace std;

ClassImp(THealpixMap);

 ///////////////////////////////////////////////////////////////////////
 //                                                    
 //  This class is a wrap of a Healpix map code. It allows you for 
 //  example to save Healpix maps to ROOT files and use Healpix code
 //  in ROOT's C++ interpreter. Thsi can be very useful when using 
 //  TSelector class to analise data.
 //                                                       
 //  NOTE: Some member functions are still not implemented.
 //                                                 
 ///////////////////////////////////////////////////////////////////////

THealpixMap::THealpixMap(Int_t J): fJ(J), fNSide(Power(2,fJ)), 
fNPix(12*Power(fNSide,2)), fWeightLength(2*fNSide),fWeightVal(1)
{
   // Healpix Nside parameter will be 2^J. This value will also be used 
   // by the member ::CreateAlm() to set TAlm band limits.


   // Is it safe to use this pointer in a constructor?
   Set(fNPix);
}

//______________________________________________________________
void THealpixMap::Fill(const TF1& f)
{
   // Set Pixel values using value o function f.

   Healpix_Map<Double_t> map(fNSide,RING,SET_NSIDE);

   for (Int_t i = 0; i < fNPix; ++i){
      pointing ang = map.pix2ang(i);
      fArray[i] = f.Eval(ang.phi,ang.theta);
   }
}

//______________________________________________________________
void THealpixMap::Fill(Double_t& theta,Double_t& phi)
{
   // Fill a Healpix map by on event in the corresponding pixel.

   pointing ang(theta,phi); 
   Healpix_Map<Double_t> mapa(fNSide,RING,SET_NSIDE);
   ++fArray[mapa.ang2pix(ang)];
}

//________________________________________________________________
void THealpixMap::Copy(TObject& rhs) const
{
   // Copy this map to new one

   TArrayD& arr = dynamic_cast<TArrayD&>(rhs);
   TVMap::Copy(rhs);
   TArrayD::Copy(arr);
   ((THealpixMap&)rhs).fJ = fJ;
   ((THealpixMap&)rhs).fNPix  = fNPix;
   ((THealpixMap&)rhs).fNSide = fNSide;
   ((THealpixMap&)rhs).fWeightLength = fWeightLength;
   ((THealpixMap&)rhs).fWeightVal = fWeightVal;
}

void THealpixMap::Filter(Double_t factor)
{
   // Has to be implemented
}

void THealpixMap::FindSources(Int_t nsources,Double_t r) const
{
   // Has to be implemented.
}

//_________________________________________________
void THealpixMap::CreateAlm(TAlm& harm) const
{
   // Performs a spherical harmonic transform.

   Healpix_Map<Double_t> map(fNSide,RING,SET_NSIDE);
   arr<Double_t> apix;
   apix.alloc(fNPix);

   for (Int_t i = 0; i < fNPix; ++i)
      apix[i] = fArray[i];

   map.Set(apix,RING);
   arr<Double_t> weight(fWeightLength,fWeightVal);
   Alm<xcomplex<Double_t> > alm1(fNSide,fNSide);
   map2alm<Double_t>(map,alm1,weight);

   Int_t L = harm.GetL();

   for (Int_t l = 0; l < L; ++l){
      for (Int_t m = 0; m <= l; ++m){
	 xcomplex<double> c = alm1(l,m);
	 harm(l,m) = complex<Double_t>(c.real(),c.imag());
      }
   }
}

//_________________________________________________
TH2D* THealpixMap::CreateHist(Int_t) const
{
   // Creates a histogram for visualisation.

   TH2D* h = new TH2D("","histogram",fNSide,0,2*Pi(),fNSide,0,Pi());

   Healpix_Map<Double_t> map(fNSide,RING,SET_NSIDE);

   for (Int_t i = 0; i < fNPix; ++i){
      pointing ang = map.pix2ang(i);
      h->SetBinContent(h->FindBin(ang.phi,ang.theta),fArray[i]);
   }

   h->GetXaxis()->SetLimits(-180,180);
   h->GetYaxis()->SetLimits(-90,90);

   return h;
}

void THealpixMap::Draw(Option_t* option)
{
   // Draws the map.

   TH2D* h = CreateHist(0);
   h->Draw(option);
}





