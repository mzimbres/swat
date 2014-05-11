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

#include <vector>
#include <complex>
#include <cmath>

// ROOT
#include "TString.h"

#include <swat/swat.h>
#include <swat/TWignerd.h>
#include <swat/TAlm.h>
#include <swat/TSphHarmB.h>
#include <swat/TSkyMap.h>

using namespace std;
using namespace TMath;

 ///////////////////////////////////////////////////////////////////////
 //                                                                   //
 //  Calculates spherical harmonic coefficients                       //
 //                                                                   //
 //                                                                   //
 ///////////////////////////////////////////////////////////////////////

//______________________________________________________________
TSphHarmB::TSphHarmB(int J,const char* opt):fJmax(J),
fL(kAlpha << (fJmax - 1)),fNPhi(fL << 1),fNTheta(fL << 2),
fSize(fNPhi*fNTheta),fWeightVal(fNTheta >> 1),fShift(fNPhi-1)
{
   // The signal to be set must be compatible with
   // the values provided in this constructor. It means 
   // it must have the same jmax.
   // opt: can be "ES", "M", "P" and "EX". Same as in FFTW

   dim[0] = fNTheta;
   dim[1] = fNPhi;

   TString ss("R2C ");
   ss.Append(opt);
   ss.Append(" K");
   B.reset(TVirtualFFT::FFT(2,dim,ss.Data()));

   for(size_t u=0; u < fWeightVal.size(); ++u)
      fWeightVal[u] = fWeights(u);
   
   for (int u= -(fL-1);u < fL;++u){
      complex<double> shift(0,-u*pie/fNTheta);
      fShift[fShiftIndex(u)] = exp(shift);
   }
}

//______________________________________________________________
void TSphHarmB::GetAlm(TAlm& alm) const
{
   // Get Harmonic coeficients

   TWignerd Wig(fL);

   for(int l = 0; l < fL; ++l, Wig.Recurse()){
      double sec  = sqrt(((l << 1) + 1)*pie);
      for(int m = 0; m <= l; ++m){
	 alm(l,m) = fWigCoeff(Wig,m,0)*sec;
      }
   }
}

//_________________________________________________________
void TSphHarmB::SetPoints(const TSkyMap& re)
{
   // Set wavelet coefficients.

   std::vector<double> temp(fSize);

   for (int j = 0;j < fNPhi; ++j)
      for (int i = 0; i < fNPhi; ++i)
	 temp[fIndex2(i,j)] = re(i,j)*fWeightVal[j]/fNPhi;

   B->SetPoints(&temp[0]);
}

//_________________________________________________________
std::complex<double> TSphHarmB::fWigCoeff(TWignerd &w,int m,int n) const
{

   int l = w.GetL();

   double re, im;
   complex<double> first;
   for (int u = -l; u <= l; ++u){ 
      B->GetPointComplex(fIndex(m,u),re,im);
      first += complex<double>(re,im)*w(m,n,-u)*fShift[fShiftIndex(u)];
   }

   return first;
}

//________________________________________________________________
double TSphHarmB::fWeights(int k)
{
   // Returns the quadrature weights

   double a = 2*sin(pie*((k << 1) + 1)/(fL << 2))/fL;

   double b(0);

   for(int i=0;i<fL;++i)
      b += sin(((i << 1) + 1)*((k << 1) + 1)*pie/(fL << 2))/((i << 1) + 1);
   
   return a*b;
}

