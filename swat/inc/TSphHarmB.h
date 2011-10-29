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

#ifndef SWAT_TSphHarmB
#define SWAT_TSphHarmB

#include <vector>
#include <memory>
#include <complex>

// ROOT

#include "TVirtualFFT.h"

// SWAT

class TAlm;
class TWignerd;
class TSkyMap;

class TSphHarmB {
   private:
   int fJmax;   // Maximum number of scales.
   int fL;      // Zenital band limit.
   int fNPhi;   // Number of points in phi.
   int fNTheta; // Number of points in theta.
   int fSize;   // Size of array with wavelet coefficients.
   int dim[2];  // Array with the size of each dimension.
   int fIndex(int m,int u) const {
      return (u < 0) ? (m + (fL + 1)*(fNTheta+u)) : (m+(fL+1)*u);
   }
   std::complex<double> fWigCoeff(TWignerd &w,int m, int n) const;
   int        fIndex2(int m,int u) const {return  m+fNPhi*u;}
   double     fWeights(int k);
   std::auto_ptr<TVirtualFFT> B;
   std::vector<double> fWeightVal; // Stores quadrature weights.
   int        fShiftIndex(int u) const {
      return (u < 0) ? (fNPhi-1+u) : u;
   }
   std::vector<std::complex<double> > fShift;     // Shifts

   public:
   explicit TSphHarmB(int J,const char* opt = "M");
   void     GetAlm(TAlm& alm) const;
   void     SetPoints(const TSkyMap& re);
   void     Transform() { B->Transform(); }
};

#endif // SWAT_TSphHarmB

