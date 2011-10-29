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

#ifndef SWAT_TAlm
#define SWAT_TAlm

#include <vector>
#include <complex>

class TAlm {
   private:
   int     fJmax;      // Maximum number of Scales.
   size_t  fL;         // Band limit: l < fL
   size_t  fSize;      // Size necessary to store data.
   std::vector<std::complex<double> > fAlm;  // Stores real part
   int    fIndex(size_t l,size_t m) const {return ((l*(l+1) >> 1) + m);} // Global index

   public:
   explicit TAlm(int J): fJmax(J), fL(2 << (fJmax - 1)), fSize(((fL*(fL+1)) >> 1) + fL), fAlm(fSize) { };
   std::complex<double>  operator()(int l,int m) const {return fAlm[fIndex(l,m)];}
   std::complex<double>& operator()(int l,int m) {return fAlm[fIndex(l,m)];}
   void   Add(const TAlm& alm);
   int    GetJmax() const {return fJmax;}
   size_t GetL() const {return fL;}
   void   Conjugate();
   void   ScaleL(const std::vector<double>& d);
   void   Scale(double factor);
};

#endif

