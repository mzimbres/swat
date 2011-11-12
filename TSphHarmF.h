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

#ifndef SWAT_TSphHarmF
#define SWAT_TSphHarmF

#include <memory>

// ROOT

#include "TVirtualFFT.h"

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TShift.h"

class TSkyMap;
class TAlm;

class TSphHarmF {
   private:
   TCoeffInfo   fInfo;
   TShift       fShift;
   int        dim[2];      // Array with the size of each dimension.
   std::auto_ptr<TVirtualFFT> B;
				     
   public:
   explicit TSphHarmF(int J,const char* opt = "M");
   void     SetPoints(const TAlm& alm);
   TSkyMap* GetPoints() const;
   void     Transform();
};

#endif // SWAT_TSphHarmF

