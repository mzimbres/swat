// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010, 2011, 2012 Marcelo Zimbres Silva
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

#ifndef SWAT_TSmallWignerd
#define SWAT_TSmallWignerd

#include <memory>

// ROOT

#include "TVirtualFFT.h"

// SWAT

class TWignerd;

class TSmallWignerd {
   private:
   Int_t fSize;
   Int_t fEvenType;
   Int_t fOddType;
   std::auto_ptr<TVirtualFFT> fEven;
   std::auto_ptr<TVirtualFFT> fOdd;

   public:
   explicit TSmallWignerd(Int_t L)
   : fSize(2*L), fEvenType(1), fOddType(5),
     fEven(TVirtualFFT::SineCosine(1,&fSize,&fEvenType,"M K")),
     fOdd(TVirtualFFT::SineCosine(1,&fSize,&fOddType,"M K")) { }
   Int_t GetSize() const {return fSize;}
   Double_t* Get(const TWignerd& wig,Int_t m,Int_t n) const;
};

#endif

