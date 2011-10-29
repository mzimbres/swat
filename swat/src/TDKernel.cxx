// Author: Marcelo Zimbres Silva 2009

/* Copyright (C) 2009, 2011, Marcelo Zimbres Silva
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
#include <vector>
#include <algorithm>

// ROOT

#include "TMath.h"
#include "TArrayD.h"

// SWAT

#include "swat.h"
#include "TSFunc.h"
#include "TDKernel.h"

using namespace TMath;

   ///////////////////////////////////////////////////////////
   //                                                       //
   //                                                       //
   //   Class to calculate the kernel of the wavelet        //
   //                                                       //
   //                                                       //
   ///////////////////////////////////////////////////////////

//_________________________________________________________________________
TDKernel::TDKernel(Int_t j,Int_t J):fScale(j),fJmax(J),
fBandLim(Power(kAlpha,fJmax)),fBegin(Power(kAlpha,J-fScale-1)),
fEnd(kAlpha << (J-fScale))
{
   // Constructor
   // j: Will calculate the kernel in this scale 
   // jmax: Maximum number of scales

   if (fEnd > fBandLim) fEnd = fBandLim;

   if (fScale > fJmax)
      throw "TDKernel j <= J";
   
}


//_________________________________________________________________________
TArrayD TDKernel::GetKernel() const
{
   // Returns a vector containing kernel values for all frequencies
   // in the range (0,B) where B is the band limit.

   TSFunc sf(fJmax);

   std::vector<Double_t> vec;
   vec.resize(fBandLim);

   if(fScale==0){
      vec = sf.GetSquaredAtScale(fScale);
      for(Int_t i=0;i<fBandLim;++i){
         vec[i] = 1-vec[i];
      } 
   } else { 
      std::vector<Double_t> a = sf.GetSquaredAtScale(fScale);
      std::vector<Double_t> b = sf.GetSquaredAtScale(fScale-1);
      for(Int_t i=0;i<fBandLim;++i){ 
         vec[i] = Abs(a[i]-b[i]); 
      }
   }
   std::transform(vec.begin(),vec.end(),vec.begin(),Sqrt);
   TArrayD arr(fBandLim,&vec[0]);
   return arr;
}

//______________________________________________________
void TDKernel::Copy(TDKernel& kernel) const
{
   // Copies this to kernel.

   kernel.fScale = fScale;
   kernel.fJmax = fJmax;
   kernel.fBandLim = fBandLim;
   kernel.fBegin = fBegin;
   kernel.fEnd = fEnd;
}






