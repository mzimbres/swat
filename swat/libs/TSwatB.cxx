// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2009, 2010, 2011 Marcelo Zimbres Silva
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

#include "TMath.h"
#include "TString.h"

// SWAT

#include <swat/swat/swat.h>
#include <swat/swat/TWignerd.h>
#include <swat/swat/TAlm.h>
#include <swat/swat/TSwatB.h>
#include <swat/swat/TDKernel.h>
#include <swat/swat/TWavMap.h>
#include <swat/swat/TWavelet.h>

using namespace TMath;
using namespace std;

 ///////////////////////////////////////////////////////////////////////
 //                                                                   //
 //                                                                   //
 //  Transform wavelet coefficients to spherical harmonic space.      //
 //                                                                   //
 //                                                                   //
 //                                                                   //
 ///////////////////////////////////////////////////////////////////////

//_________________________________________________________________________
TSwatB::TSwatB(Int_t J,Int_t N,const char* opt):fInfo(J,N),fShift(J,N,-1),
fQWeights(fInfo.GetNTheta()/2)
{
   // The signal to be set must be compatible with
   // the values provided in this constructor. It means 
   // it must have the same jmax.
   // opt: can be "ES", "M", "P" and "EX". Same as in FFTW

   generate(fQWeights.begin(),fQWeights.end(),TQWCalc(fInfo.GetL()));

   dim[0] = fInfo.GetNQui();
   dim[1] = fInfo.GetNTheta();
   dim[2] = fInfo.GetNPhi();

   TString ss("R2C ");
   ss.Append(opt);
   ss.Append(" K");
   fB.reset(TVirtualFFT::FFT(3,dim,ss.Data()));
}

//_________________________________________________________
void TSwatB::SetPoints(const TWavMap& re)
{
   // Set wavelet coefficients.

   std::vector<double> temp(fInfo.SizeCoordinate());
   Int_t rate = fInfo.GetNTheta()*fInfo.GetNQui();

   Int_t half = fInfo.GetNTheta()/2;

   for(Int_t n=0;n<fInfo.GetNQui();++n)
      for(Int_t u=0;u<half;++u)
	 for(Int_t m=0;m<fInfo.GetNPhi();++m)
            temp[fInfo.Coordinate(m,u,n)] = re(m,u,n)*fQWeights[u]/rate;

   fB->SetPoints(&temp[0]);
}

//______________________________________________________________
void TSwatB::Transform()
{
   // Transform to harmonic space

   fB->Transform();
}

//______________________________________________________________
void TSwatB::GetAlm(TAlm& alm,Int_t j) const
{
   // Get Harmonic coeficients

   TAlm wav(fInfo.GetJ());
   TWavelet::fill_wav(wav,j,fInfo.GetN());

   TDKernel ker(j,fInfo.GetJ());
   Int_t a = ker.Begin();
   Int_t b = ker.End();

   TWignerd Wig(fInfo.GetL());
   Wig.Advance(a+1);

   for (Int_t l = a + 1; l < b; ++l, Wig.Recurse()){
      Double_t xx  = 2*l+1;
      Int_t lim = Min(fInfo.GetN()-1,l);
      for (Int_t m = 0; m <= l; ++m){
         complex<Double_t> third = wav(l,0)*fWigCoeff(Wig,m,0);
         for (Int_t n = 1; n <= lim; ++n)
	    if (Odd(fInfo.GetN() + n))
	       third += 2*real(wav(l,n))*fWigCoeff(Wig,m,n);
         third *= xx;
         alm(l,m) = third;
      }
   }
}

Double_t TSwatB::TQWCalc::operator()() 
{
      // Returns the quadrature weights

      Double_t a = 2*Sin(Pi()*(2*fK+1)/(4*fL))/fL;

      Double_t b(0);

      for (Int_t i = 0; i < fL; ++i)
	 b += Sin((2*i+1)*(2*fK+1)*Pi()/(4*fL))/(2*i+1);

      ++fK;
      
      return a*b;
}

