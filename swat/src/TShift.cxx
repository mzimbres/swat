// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 Marcelo Zimbres Silva
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

// SWAT

#include "swat.h" 
#include "TShift.h" 

using namespace TMath;

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //    
   //   Provides shifts in Fourier Domain
   //                                                        // 
   //                                                        // 
   ////////////////////////////////////////////////////////////

//______________________________________________________________
TShift::TShift(Int_t J,Int_t N,Int_t t):fInfo(J,N),
fShifts(fInfo.GetNPhi()-1)
{
   // Fill array of TComplex with shift values.

   Int_t L = fInfo.GetL();

   for (Int_t u = -(L-1);u < L;++u){
      std::complex<Double_t> shift(0,t*Pi()*u/fInfo.GetNTheta());
      fShifts[fShiftIndex(u)] = exp(shift);
   }
}

