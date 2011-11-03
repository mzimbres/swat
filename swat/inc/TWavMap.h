// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010, 2011 Marcelo Zimbres Silva
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

#ifndef SWAT_TWavMap
#define SWAT_TWavMap

#include <functional>
#include <cmath>
#include <vector>


class TF1;
class TH2D;

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TDKernel.h"

class TAlm;

class TWavMap: public TCoeffInfo, public TDKernel {
   private:
   int fOrientation;
   template<typename T>
   struct less_abs: std::binary_function<T,T,bool>{
      bool operator()(T x,T y) const { return std::abs(x) < std::abs(y); }
   };

   public:
	     std::vector<double> fArray; // The Data
             TWavMap(int j,int J,int N = 1): 
	     TCoeffInfo(J,N), TDKernel(j,J), fArray(fSizeCoordinate), fOrientation(0)  {}
   double& operator()(int i,int j,int k) {return fArray[Coordinate(i,j,k)];}
   double  operator()(int i,int j,int k) const {return fArray[Coordinate(i,j,k)];}
   void      Filter(double factor);
   void      SetOrientation(int i) {fOrientation = i;}
   void      FindSources(int nsources,double r) const;
   void      CreateAlm(TAlm& alm) const;
   TH2D*     CreateHist(int n) const;
   void      Fill(const TF1& f);
};

#endif // SWAT_TWavMap

