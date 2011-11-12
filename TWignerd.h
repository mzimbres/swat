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

class TWignerd {
   private:
   int fB;         // Band limit of the signal to be analised.
   int fL;             // Value of l in each step of the recursion.
   int fSize;                    // Size of container with deltas.
   std::vector<double>   fMatrix;  // Delta coefficients. 		   
   int fIndex(int m,int n) const {return (((m*(m+1)) >> 1) + n);} // Global index

   public:
   explicit TWignerd(int B): fB(B+1), fL(0), 
   fSize(((fB*(fB+1)) >> 1) + fB),fMatrix(fSize) { fMatrix[fIndex(0,0)] = 1; }

   void     Recurse(); 
   std::complex<double> operator()(int m,int n,int u) const;
   double Delta(int m,int n) const;
   void   Advance(int l);
   int    GetL() const {return fL;}
};

//_____________________________________________________________________
inline std::complex<double> TWignerd::operator()(int m,int n,int u) const
{
   // Returns Fourier coefficients of the wigner d-function d^l_mn.
   // If module of m or n or u greater than l returns zero instead of 
   // reporting an error.
   // u: frequency

   if ( (m - n) & 1) 
      return ((m - n + 1) & 3) ? -std::complex<double>(0,1)*Delta(u,m)*Delta(u,n): std::complex<double>(0,1)*Delta(u,m)*Delta(u,n);
   return ((m - n) & 3) ? -std::complex<double>(1,0)*Delta(u,m)*Delta(u,n): std::complex<double>(1,0)*Delta(u,m)*Delta(u,n) ;
}

//__________________________________________________________________________
inline double TWignerd::Delta(int m,int n) const
{
   // Returns delta using symmetry relations.

   if (m < 0) {
      if (n < 0) {
         if (m >= n)
            return fMatrix[fIndex(-n,-m)];
         return ((n-m) & 1) ? -fMatrix[fIndex(-m,-n)]: fMatrix[fIndex(-m,-n)];
      } else {
         if ((n + m) <= 0)
            return ((fL-n) & 1) ? -fMatrix[fIndex(-m,n)]: fMatrix[fIndex(-m,n)];
	 return ((fL-m) & 1) ? -fMatrix[fIndex(n,-m)]: fMatrix[fIndex(n,-m)];
      }
   }
   if (n < 0) {
      if ((m + n) >= 0)
         return ((fL-m) & 1) ? -fMatrix[fIndex(m,-n)]: fMatrix[fIndex(m,-n)];
      return ((fL-n) & 1) ? -fMatrix[fIndex(-n,m)]: fMatrix[fIndex(-n,m)];
   } else {
      if (m >= n)
         return fMatrix[fIndex(m,n)];
      return ((n-m) & 1) ? -fMatrix[fIndex(n,m)]: fMatrix[fIndex(n,m)];
   }
}

#endif

