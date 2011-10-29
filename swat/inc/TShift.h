// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 Marcelo Zimbres Silva
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

#ifndef SWAT_TShift
#define SWAT_TShift

// ROOT

#include <complex>

// SWAT

#include "TCoeffInfo.h" 

class TShift {
   private:
   TCoeffInfo fInfo;
   std::vector<std::complex<double> > fShifts; // Shifts
   int fShiftIndex(int u) const 
   {
      return u < 0 ? fInfo.GetNPhi() - 1 + u : u; 
   }

   public:
            TShift(int J,int N = 1,int t = 1);
   std::complex<double> operator()(int u) const {return fShifts[fShiftIndex(u)];}
   std::complex<double>& operator()(int u) {return fShifts[fShiftIndex(u)];}
};

#endif // SWAT_TShift

