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

#ifndef SWAT_TSwatB
#define SWAT_TSwatB

#include <memory>
#include <vector>
#include <complex>

// ROOT

#include "TVirtualFFT.h"
#include "TAlm.h"

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TShift.h"

class TAlm;
class TWignerd;
class TWavMap;
 
class TSwatB {
   private:
   TCoeffInfo   fInfo;
   TShift       fShift;
   std::vector<double> fQWeights;
   Int_t        dim[3];      // Array with the size of each dimension.
   std::complex<Double_t> fWigCoeff(TWignerd &w,Int_t m, Int_t n) const;
   std::auto_ptr<TVirtualFFT> fB;     //! No streamer available.
   class TQWCalc{
      private:
      Int_t fL;
      Int_t fK;
      public:
      TQWCalc(Int_t L): fL(L), fK(0) { }
      Double_t operator()();
   };
   TSwatB& operator=(const TSwatB& obj);
   TSwatB(const TSwatB& obj);

   public:
         TSwatB(Int_t J,Int_t N,const char* opt = "M");
   void  GetAlm(TAlm& alm,Int_t j) const;
   void  SetPoints(const TWavMap& re);
   void  Transform();
};

#endif // SWAT_TSwatB

