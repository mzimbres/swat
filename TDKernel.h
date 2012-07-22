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

#include <vector>

// SWAT

#include "swat.h"

class TDKernel {
   private:
   int fJmax;  // Number of scales that will be used
   protected:
   int fScale; // The scale to construct the kernel. Range [0,fJmax]
   private:
   int fBandLim;     // This is the band limit. Is determined by fJmax
   int fBegin; // Support begin
   int fEnd;   // Support end
   
   public:
           TDKernel(int j,int J):fJmax(J), 
	      fScale((j <= fJmax) ? j : throw "TDKernel: j > J."),
	      fBandLim((kAlpha << (fJmax - 1))),
              fBegin(((J - fScale < 2)) ? 0 : kAlpha << (J-fScale-2)),
	      fEnd((j == 0) ? fBandLim : (kAlpha << (J - fScale))) { }
   void  Copy(TDKernel& kernel) const;
   int   GetLmax() const {return fBandLim;}
   int   Begin() const {return fBegin;}
   int   End() const {return fEnd;}
   int   GetScale() const {return fScale;}
   std::vector<double> GetKernel() const;
};

#endif

