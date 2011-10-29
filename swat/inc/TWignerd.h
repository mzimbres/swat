// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2009 Marcelo Zimbres Silva
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

#ifndef SWAT_TWignerd
#define SWAT_TWignerd

#include <complex>
#include <vector>

// ROOT

#include "TMath.h"
 
class TWignerd {
   private:
   Int_t fLBandLim;         // Band limit of the signal to be analised.
   Double_t fL;             // Value of l in each step of the recursion.
   Int_t fSize;                    // Size of container with deltas.
   std::vector<double>   fMatrix;  // Delta coefficients. 		   
   Int_t    fIndex(Int_t m,Int_t n) const {return (((m*(m+1)) >> 1) + n);} // Global index

   public:
            TWignerd(Int_t lbandlim); 
   void     Recurse(); 
   std::complex<Double_t> operator()(Int_t m,Int_t n,Int_t u) const;
   Double_t Delta(Int_t m,Int_t n) const;
   void     Advance(Int_t l);
   Int_t    GetL() const {return fL;}
};

//_____________________________________________________________________
inline std::complex<Double_t> TWignerd::operator()(Int_t m,Int_t n,Int_t u) const
{
   // Returns Fourier coefficients of the wigner d-function d^l_mn.
   // If module of m or n or u greater than l returns zero instead of 
   // reporting an error.
   // u: frequency

   if ( (m - n) & 1) 
      return ((m - n + 1) & 3) ? -std::complex<Double_t>(0,1)*Delta(u,m)*Delta(u,n): std::complex<Double_t>(0,1)*Delta(u,m)*Delta(u,n);
   return ((m - n) & 3) ? -std::complex<Double_t>(1,0)*Delta(u,m)*Delta(u,n): std::complex<Double_t>(1,0)*Delta(u,m)*Delta(u,n) ;
}

//__________________________________________________________________________
inline Double_t TWignerd::Delta(Int_t m,Int_t n) const
{
   // Returns delta using symmetry relations.

   if (m < 0) {
      if (n < 0) {
         if (m >= n)
            return fMatrix[fIndex(-n,-m)];
         return TMath::Even(n-m) ? fMatrix[fIndex(-m,-n)]: -fMatrix[fIndex(-m,-n)];
      } else {
         if (n <= TMath::Abs(m))
            return TMath::Even(fL-n) ? fMatrix[fIndex(-m,n)]: -fMatrix[fIndex(-m,n)];
	 return TMath::Even(fL-m) ? fMatrix[fIndex(n,-m)]: -fMatrix[fIndex(n,-m)];
      }
   }
   if (n < 0) {
      if (m >= TMath::Abs(n))
         return TMath::Even(fL-m) ? fMatrix[fIndex(m,-n)]: -fMatrix[fIndex(m,-n)];
      return TMath::Even(fL-n) ? fMatrix[fIndex(-n,m)]: -fMatrix[fIndex(-n,m)];
   } else {
      if (m >= n)
         return fMatrix[fIndex(m,n)];
      return TMath::Even(n-m) ? fMatrix[fIndex(n,m)]: -fMatrix[fIndex(n,m)];
   }
}

#endif

