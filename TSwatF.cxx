// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2009, 2010, 2011, Marcelo Zimbres Silva
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

#include <iostream>
#include <exception>
#include <vector>
#include <complex>

// ROOT

#include "TMath.h"
#include "TString.h"

// SWAT

#include "TWignerd.h" 
#include "TAlm.h" 
#include "TSwatF.h" 
#include "TWavMap.h" 
#include "TDKernel.h"
#include "TWavelet.h"

using namespace TMath;
using namespace std;

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //    
   //   Transform from harmonic to wavelet space             
   //   In the tutorials you can find out how to use it.     
   //
   //                                                        // 
   ////////////////////////////////////////////////////////////

//______________________________________________________________
TSwatF::TSwatF(Int_t J,Int_t N,const char* opt):
fInfo(J,N),fShift(J,N)
{
   // The signal to be set must be compatible with
   // the values provided in this constructor. It means 
   // it must have the same jmax.
   // opt: can be "ES", "M", "P" and "EX". Which have the 
   // same meaning as in FFTW.

   dim[0] = fInfo.GetNQui();
   dim[1] = fInfo.GetNTheta();
   dim[2] = fInfo.GetNPhi();

   TString ss("C2R ");
   ss.Append(opt);
   ss.Append(" K");

   B.reset(TVirtualFFT::FFT(3,dim,ss.Data()));
}

//______________________________________________________________
TWavMap* TSwatF::GetPoints() const
{
   // Get wavelet Coefficients. 

   TWavMap* wav = new TWavMap(fCurrentScale,fInfo.GetJ(),fInfo.GetN());
   double* first = B->GetPointsReal();
   std::copy(first,first + fInfo.SizeCoordinate(),wav->fArray.begin());

   return wav;
}

//_________________________________________________________
void TSwatF::SetPoints(const TAlm& alm,Int_t j)
{
   // Set harmonic coefficients to be transformed to wavelet space.
   // Its L band limit and N band limit have to be the same.

   fCurrentScale = j;

   try {
      vector<Double_t> re(fInfo.SizeFourier());
      vector<Double_t> im(fInfo.SizeFourier());
   
      TAlm Wav(fInfo.GetJ());
      TWavelet::fill_wav(Wav,j,fInfo.GetN());
      Wav.Conjugate();

      TDKernel ker(j,fInfo.GetJ()); // Provides kernel support.

      TWignerd Wig(fInfo.GetL());
      Wig.Advance(ker.Begin());

      for (Int_t l = ker.Begin(); l < ker.End(); ++l, Wig.Recurse()){
         Int_t lim = Min(fInfo.GetN()-1,l);

         for (Int_t n = -lim; n <= 0; ++n){
            if (Odd(fInfo.GetN() + n)) {
               complex<Double_t> c = Wav(l,-n);
	       c = fGetNegativen(c,n);
               for(Int_t u = -l; u<= l; ++u){ 
                  for(Int_t m = 0; m <= l; ++m){
                     complex<Double_t> f = c*alm(l,m)*Wig(m,n,u)*fShift(u);
                     re[fInfo.Fourier(m,u,n)] += real(f);
                     im[fInfo.Fourier(m,u,n)] += imag(f);
                  }
               }
            }
         }

         for (Int_t n = 1; n <= lim; ++n){
            if (Odd(fInfo.GetN() + n)) {
               complex<Double_t> c = Wav(l,n);
               for(Int_t u = -l; u<= l; ++u){ 
                  for(Int_t m = 0; m <= l; ++m){
                     complex<Double_t> f = c*alm(l,m)*Wig(m,n,u)*fShift(u);
                     re[fInfo.Fourier(m,u,n)] += real(f);
                     im[fInfo.Fourier(m,u,n)] += imag(f);
                  }
               }
            }
         }

      }
      B->SetPointsComplex(&re[0],&im[0]);

   } catch (std::exception& e) {
      cout << "Exception caught at TSwatF::SetPoints. " << endl;
      cout << "You are probably using too much memory. " << endl;
      terminate();
   }
}

