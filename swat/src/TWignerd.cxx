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
#include <cmath>

// SWAT

#include "TWignerd.h"

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

//_______________________________________________________________
void TWignerd::Recurse()
{
   // Advances one step in the recursion, from l to l+1

   ++fL;
   std::vector<double> fTemp(fSize); 

   fTemp[fIndex(fL,0)] = -sqrt(1 - 0.5/fL)*fMatrix[fIndex(fL-1,0)];

   for (int n = 1; n <= fL; ++n){
      double a = sqrt((fL*(fL - 0.5))/((fL+n)*(fL+n-1)));
      fTemp[fIndex(fL,n)] = a*fMatrix[fIndex(fL-1,n-1)];
   }
   
   for(int m=fL-1;m>=0;--m){
      for(int n=0;n<=m;++n){
         double a = static_cast<double>(n << 1)/sqrt((fL-m)*(fL+m+1));
         double b = -sqrt((fL-m-1)*(fL+m+2.)/((fL-m)*(fL+m+1)));
   	 fTemp[fIndex(m,n)] = a*fTemp[fIndex(m+1,n)] + b*fTemp[fIndex(m+2,n)];
      }
   }

   fMatrix = fTemp;
}

//__________________________________________________________________________
void TWignerd::Advance(int l)
{
   // Advances using a loop in Recurse(), so that fL = l.

   for (int i = 0; i < l; ++i)
      Recurse();
}

