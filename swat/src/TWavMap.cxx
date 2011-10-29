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

#include <algorithm>
#include <sstream>
#include <vector>

// ROOT

#include "TArrayD.h"
#include "TMath.h"
#include "TH2D.h"
#include "TF1.h"
#include "TObject.h"
#include "TDirectory.h" 

// SWAT

#include "swat.h" 
#include "TWavMap.h" 
#include "TCoeffInfo.h" 
#include "TSwatB.h" 
#include "TEulerAngle.h" 

using namespace TMath;
using namespace std;

// Predicate used to filter coefficients.


class coord {
   private:
   Int_t    fPhiIndex;
   Int_t    fThetaIndex;
   Int_t    fQuiIndex;
   Double_t fVal;

   public:
   coord(){}
   coord(Int_t i,Int_t k,Int_t j,Double_t val):fPhiIndex(i),fThetaIndex(k),fQuiIndex(j),fVal(val){}
   Int_t    GetPhiIndex() const {return fPhiIndex;}
   Int_t    GetThetaIndex() const {return fThetaIndex;}
   Int_t    GetQuiIndex() const {return fQuiIndex;}
   Double_t GetVal() const {return fVal;}
};

class CompareAbs { 
   public:
   bool operator()(const coord& i1, const coord& i2) { 
      return TMath::Abs(i1.GetVal()) > TMath::Abs(i2.GetVal());
   }
};

class CompareResolution { 
   private:
   TCoeffInfo fInfo;
   Double_t fRes;
   Int_t fPhiPos;
   Int_t fThetaPos;

   public:
   CompareResolution(Int_t J,Int_t N,Double_t r): fInfo(J,N), fRes(r), fPhiPos(0), fThetaPos(0) {}
   void SetPhiTheta(Int_t iphi,Int_t itheta) { fPhiPos = iphi; fThetaPos = itheta;}
   bool operator()(const coord& i) const
   { 
      // returns false if i2 is less that 5 degrees of i1
      
      bool phi = Abs(fPhiPos - i.GetPhiIndex()) < TMath::Nint(fInfo.GetNPhi()*fRes/360.);
      bool theta = Abs(fThetaPos - i.GetThetaIndex()) < TMath::Nint(fInfo.GetNTheta()*fRes/360.);
      return phi || theta;
   }
};


ClassImp(TWavMap);

 ///////////////////////////////////////////////////////////////////////
 //                                                    
 //                                                      
 //                                                       
 //  Stores coefficients of functions defined on SO(3) i.e. functions
 //  of the for f(a,b,c) where 0 <= a <= pi, 0 <= b,c <= 2*pi.
 //
 //  Note: The operator oveload of () follows the convention: 
 //  First index: "i" index of the phi coordinate, range[0,fNPhi)
 //  Second index: "j" index of theta coordinate, range[0,fNTheta)
 //  Third index: "k" index of chi coordinte, range[0,fNQui)
 //                                                 
 ///////////////////////////////////////////////////////////////////////

//______________________________________________________________
void TWavMap::Filter(Double_t factor)
{
   // Will zero all coefficients which absolute values are 
   // less than factor

   //abs_less pred(factor);
   std::replace_if(&fArray[0],&fArray[0]+fSizeCoordinate,
                   bind2nd(less_abs<double>(),factor),0);
}

//_____________________________________________________________________
void TWavMap::FindSources(Int_t nsources,Double_t r) const
{
   // Adds TEulerAngles found to the current directory.
   // All coefficients that are withing r(in degrees) 
   // will be considered to belong to the same source.

   std::vector<coord> ind;
   ind.reserve(fSizeCoordinate/4);

   for (Int_t n = 0; n < TCoeffInfo::fN; ++n)
      for (Int_t u = 0;u < fNTheta/2; ++u)
	 for (Int_t m = 0; m < fNPhi; ++m){
	    coord tmp(m,u,n,fArray[Coordinate(m,u,n)]);
	    ind.push_back(tmp);
	 }
   
   std::vector<coord>::iterator head    = ind.begin();
   std::vector<coord>::iterator middle  = ind.begin();
   std::vector<coord>::iterator tail    = ind.end();

   CompareResolution tmp(fJ,TCoeffInfo::fN,r);

   std::ostringstream os;
   Int_t i = 0;
   TEulerAngle *euler = NULL;
   while (( i < nsources ) && (middle != tail)) {
      std::partial_sort(head, ++middle, tail, CompareAbs() );
      tmp.SetPhiTheta(head->GetPhiIndex(),head->GetThetaIndex());
      tail = std::remove_if(middle,tail,tmp);
      os << "source" << i;
      euler = new TEulerAngle(fJ,TCoeffInfo::fN);
      euler->SetAnglesValue(head->GetPhiIndex(),head->GetThetaIndex(),head->GetQuiIndex(),head->GetVal());
      euler->SetName(os.str().c_str());
      os.str("");
      gDirectory->Add(euler);
      ++head;
      ++i;
   }
}

//_________________________________________________________
void TWavMap::CreateAlm(TAlm& alm) const
{
   // Uses Spherical WAvelet Transform to do a backward transformation
   // to harmonic space(TAlm).

   TSwatB backward(fJ,TCoeffInfo::fN,"M");
   backward.SetPoints(*this);
   backward.Transform();
   backward.GetAlm(alm,fScale);
}

//_________________________________________________
TH2D* TWavMap::CreateHist(Int_t n) const
{
   // Creates Histogram. Angle gamma should be providee in degrees.

   TH2D* h = new TH2D("","histogram",fL,0,2*Pi(),fL,0,Pi());

   Double_t xcon = 2*Pi()/fNPhi;
   Double_t ycon = 2*Pi()/fNTheta;

   for (Int_t u = 0; u < fNPhi; ++u)
      for (Int_t m = 0; m < fNPhi; ++m){
	 Int_t bin = h->FindBin(xcon*m,ycon*u);
	 if (h->At(bin) == 0.)
	    h->SetBinContent(bin,fArray[Coordinate(m,u,n)]);
      }

   h->GetXaxis()->SetLimits(-180,180);
   h->GetYaxis()->SetLimits(-90,90);

   return h;
}

//______________________________________________________________
void TWavMap::Fill(const TF1& f)
{
   // f is used to plot the function

   for (Int_t i = 0; i < fNPhi; ++i)
      for (Int_t k = 0; k < fNPhi; ++k)
	 for (Int_t j = 0; j < fNQui; ++j)
	    fArray[Coordinate(i,k,j)] = f.Eval(i*2*Pi()/fNPhi,(2*k+1)*Pi()/fNTheta,j*2*Pi()/fNQui);
}

//________________________________________________________________
void TWavMap::Copy(TObject& rhs) const
{
   // Copy this Alm to new one

   TArrayD& arr = dynamic_cast<TArrayD&>(rhs);
   TCoeffInfo& info = dynamic_cast<TCoeffInfo&>(rhs);
   TDKernel& kernel = dynamic_cast<TDKernel&>(rhs);

   TVMap::Copy(rhs);
   TCoeffInfo::Copy(info);
   TDKernel::Copy(kernel);
   TArrayD::Copy(arr);
}

