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

#ifndef SWAT_TCoeffInfo
#define SWAT_TCoeffInfo

// ROOT

#include "Rtypes.h"

// SWAT

#include "swat.h" 

class TCoeffInfo {
   protected:
   int fJ;               // Maximum number of scales.
   int fJ1;               // Maximum number of scales.
   int fJ2;               // Maximum number of scales.
   int fL;               // Zenital band limit.
   int fN;               // Wavelet azimuthal band limit.
   int fNPhi;            // Number of points in phi.
   int fNTheta;          // Number of points in theta.
   int fNQui;            // Number of points in qui.
   int fSizeFourier;     // Size of array with wavelet coefficients.
   int fSizeCoordinate;  // Size of array with wavelet coefficients.

   public:
   explicit      TCoeffInfo(int J,int N = 1): fJ(J), fJ1(J + 1), 
                 fJ2(J + 2), fL(kAlpha << (fJ - 1)), fN(N), 
         	    fNPhi(2*fL), fNTheta(4*fL), fNQui(2*fN-1),
		    fSizeFourier(fNQui*fNTheta*(fNPhi/2 + 1)),
		    fSizeCoordinate(fNPhi*fNTheta*fNQui){};
   virtual void  Copy(TCoeffInfo& rhs) const;
           int GetJ() const {return fJ;}
           int GetL() const {return fL;}
           int GetN() const {return fN;}
           int GetNQui() const {return fNQui;}
           int GetNTheta() const {return fNTheta;}
           int GetNPhi() const {return fNPhi;}
           int SizeFourier() const {return fSizeFourier;}
           int SizeCoordinate() const {return fSizeCoordinate;}
           int Fourier(int m,int u,int n = 0) const;
           int Coordinate(int m,int u,int n = 0) const {return  m+((u+(n << fJ2)) << fJ1);} // Global index
   ClassDef(TCoeffInfo,1); // Information
};

//______________________________________________________________
inline int TCoeffInfo::Fourier(int m,int u,int n) const
{
   // Returns global index in row major order, where m is the
   // index of the last dimention, u of the second, and n first
   // dimention. The size of the last dimention is (n/2+1) 
   // since only positive frequencies have to be provided.

   if (u < 0)
      return (n < 0) ? m + (fL + 1)*(fNTheta+u + ((fNQui + n) << fJ2)) : m + (fL + 1)*(fNTheta + u + (n << fJ2));

   return (n < 0) ? m + (fL + 1)*(u + ((fNQui + n) << fJ2)) : m + (fL + 1)*(u + (n << fJ2));
}

#endif // SWAT_TCoeffInfo

