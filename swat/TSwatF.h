// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2009 2010 2011 Marcelo Zimbres Silva
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

#ifndef SWAT_TSwatF
#define SWAT_TSwatF

#include <memory>
#include <complex>

// ROOT

#include "TVirtualFFT.h"

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TShift.h"

class TWavMap;
class TAlm;

class TSwatF {
   private:
   TCoeffInfo     fInfo;          // Index and size infos
   TShift         fShift;         //! No streamer
   Int_t          dim[3];         // Array with the size of each dimension.
   std::auto_ptr<TVirtualFFT> B;  //! No streamer available
   Int_t        fCurrentScale;
   TSwatF& operator=(const TSwatF& obj);
   TSwatF(const TSwatF& obj);
   std::complex<Double_t> fGetNegativen(const std::complex<Double_t>& w,
                                        Int_t n) const 
   {
      return (n & 1) ? -conj(w) : conj(w);
   }

   public:
              TSwatF(Int_t J,Int_t N,const char* opt = "M");
   void       SetPoints(const TAlm& alm,Int_t j);
   TWavMap*   GetPoints() const;
   void       Transform() {B->Transform();};
};

#endif // SWAT_TSwatF

