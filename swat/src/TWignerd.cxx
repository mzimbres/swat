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

#include <iostream>
#include <complex>

// ROOT

#include "TMath.h"

// SWAT

#include "TWignerd.h"

using namespace TMath;
using namespace std;

 ///////////////////////////////////////////////////////////////////////
 //                                                                   //
 // Calculation of the fourier coefficients of wigner d functions     //
 // using recurrence.                                                 //
 //                                                                   //
 // Calculation of spherical harmonics and Wigner d functions by FFT. //
 // Article: Acta Crystallographica Section A, Foundations of         //
 // Crystallography.                                                  //
 // Authors: Stefano Trapani and Jorge Navasa.                        //
 //                                                                   //
 // Refer to macro wig_graph.C in tutorials directory for an example  //
 // how to use this class to sample wigner d functions.               //
 //                                                                   //
 ///////////////////////////////////////////////////////////////////////

//____________________________________________________________________
TWignerd::TWignerd(Int_t lbandlim): fLBandLim(lbandlim+1), fL(0), 
fSize(fLBandLim*(fLBandLim+1)/2 + fLBandLim),fMatrix(fSize)
{
   // lbandlim: Band limit of the signal to be analysed.

   fMatrix[fIndex(0,0)] = 1;
}

//_______________________________________________________________
void TWignerd::Recurse()
{
   // Advances one step in the recursion, from l to l+1

   ++fL;
   std::vector<double> fTemp(fSize); 

   fTemp[fIndex(fL,0)] = -Sqrt((2*fL-1)/(2*fL))*fMatrix[fIndex(fL-1,0)];

   for (Int_t n = 1; n <= fL; ++n){
      Double_t a = Sqrt(((fL/2)*(2*fL-1))/((fL+n)*(fL+n-1)));
      fTemp[fIndex(fL,n)] = a*fMatrix[fIndex(fL-1,n-1)];
   }
   
   for(Int_t m=fL-1;m>=0;--m){
      for(Int_t n=0;n<=m;++n){
         Double_t a = static_cast<Double_t>(n << 1)/Sqrt((fL-m)*(fL+m+1));
         Double_t b = -Sqrt((fL-m-1)*(fL+m+2)/((fL-m)*(fL+m+1)));
   	 fTemp[fIndex(m,n)] = a*fTemp[fIndex(m+1,n)] + b*fTemp[fIndex(m+2,n)];
         }
   }

   fMatrix = fTemp;
}

//__________________________________________________________________________
void TWignerd::Advance(Int_t l)
{
   // Advances using a loop in Recurse(), so that fL = l.

   for (Int_t i = 0; i < l; ++i)
      Recurse();
}

