// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 2011 Marcelo Zimbres Silva
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

#include <complex>
#include <vector>
#include <cmath>
#include <string>

// SWAT

#include "TWignerd.h" 
#include "TAlm.h" 
#include "TSphHarmF.h" 
#include "TSkyMap.h" 
#include "swat.h" 

using namespace std;

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //   Spherical harmonic transform a_lm -> f(\theta\phi)
   //                                                        // 
   ////////////////////////////////////////////////////////////

//______________________________________________________________
TSphHarmF::TSphHarmF(int J,const char* opt):fInfo(J),fShift(J)
{
   // The signal to be set must be compatible with
   // the values provided in this constructor. It means 
   // it must have the same jmax.
   // opt: can be "ES", "M", "P" and "EX". Which have the 
   // same meaning as in FFTW.

   dim[0] = fInfo.GetNTheta();
   dim[1] = fInfo.GetNPhi();

   string ss("C2R ");
   ss += opt;
   ss += " K";

   B.reset(TVirtualFFT::FFT(2,dim,ss.c_str()));
}

//______________________________________________________________
TSkyMap* TSphHarmF::GetPoints() const
{
   // Get wavelet Coefficients. 

   TSkyMap* wav = new TSkyMap(fInfo.GetJ());
   wav->Set(fInfo.SizeCoordinate(),B->GetPointsReal());

   return wav;
}

//_________________________________________________________
void TSphHarmF::SetPoints(const TAlm& alm)
{
   // Set harmonic coefficients to be transformed

   if(fInfo.GetL() != alm.GetL()){
      Error("SetPoints","Signal not compatible with the transform.");
      return;
   }

   vector<double> re(fInfo.SizeFourier());
   vector<double> im(fInfo.SizeFourier());

   TWignerd Wig(fInfo.GetL());

   for (int l = 0; l < fInfo.GetL(); ++l, Wig.Recurse()){
      double sec  = sqrt((2*l+1)/(4*pie));
      for (int u = -l; u <= l; ++u){ 
         for(int m = 0; m <= l; ++m){
            complex<double> f = alm(l,m)*Wig(m,0,u)*fShift(u);
            re[fInfo.Fourier(m,u)] += sec*real(f);
            im[fInfo.Fourier(m,u)] += sec*imag(f);
         }
      }
   }

   B->SetPointsComplex(&re[0],&im[0]);
}

//______________________________________________________________
void TSphHarmF::Transform()
{
   // Transform to wavelet space

   B->Transform();
}

