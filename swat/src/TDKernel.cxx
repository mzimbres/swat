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
#include <cmath>

// SWAT

#include "TSFunc.h"
#include "TDKernel.h"

using namespace std;

   ///////////////////////////////////////////////////////////
   //                                                       //
   //                                                       //
   //   Class to calculate the kernel of the wavelet        //
   //                                                       //
   //                                                       //
   ///////////////////////////////////////////////////////////

//_________________________________________________________________________
std::vector<double> TDKernel::GetKernel() const
{
   // Returns a vector containing kernel values for all frequencies
   // in the range (0,B) where B is the band limit.

   TSFunc sf(fJmax);

   std::vector<double> vec;
   vec.resize(fBandLim);

   if(fScale==0){
      vec = sf.GetSquaredAtScale(fScale);
      for(int i = 0; i < fBandLim; ++i){
         vec[i] = 1-vec[i];
      } 
   } else { 
      std::vector<double> a = sf.GetSquaredAtScale(fScale);
      std::vector<double> b = sf.GetSquaredAtScale(fScale-1);
      for(int i=0; i < fBandLim; ++i){ 
         vec[i] = abs(a[i]-b[i]); 
      }
   }
   std::transform(vec.begin(),vec.end(),vec.begin(),
                  std::ptr_fun<double, double>(sqrt));
   return vec;
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

