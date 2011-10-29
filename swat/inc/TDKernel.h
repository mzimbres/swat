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

#ifndef SWAT_TDKernel
#define SWAT_TDKernel

// ROOT

#include "TArrayD.h"

class TDKernel {
   protected:
   Int_t fScale; // The scale to construct the kernel. Range [0,fJmax]
   private:
   Int_t fJmax;  // Number of scales that will be used
   Int_t fBandLim;     // This is the band limit. Is determined by fJmax
   Int_t fBegin; // Support begin
   Int_t fEnd;   // Support end
   
   public:
           TDKernel(Int_t j,Int_t J);
   void    Copy(TDKernel& kernel) const;
   Int_t   GetLmax() const {return fBandLim;}
   Int_t   Begin() const {return fBegin;}
   Int_t   End() const {return fEnd;}
   TArrayD GetKernel() const;
};

#endif

