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

#include <iostream>
#include <cmath>

// ROOT

#include "TArrayD.h"
#include "TH2D.h"
#include "TF1.h"
#include "TObject.h"
#include "TMath.h"

// SWAT

#include "swat.h" 
#include "TSkyMap.h" 
#include "TCoeffInfo.h" 
#include "TSphHarmB.h" 

using namespace TMath;
using namespace std;


class abs_less{
   private:
   Double_t fThreshold;
   public:
   abs_less(Double_t T): fThreshold(T){}
   bool operator()(Double_t val) const {return (TMath::Abs(val) < fThreshold) ? kTRUE: kFALSE;}
};

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
      return phi && theta;
   }
};


ClassImp(TSkyMap);


 ///////////////////////////////////////////////////////////////////////
 //                                                    
 //                                                      
 //                                                       
 //  Stores coefficients of functions defined on S2 i.e. functions
 //  of the for f(a,b,c) where 0 <= a <= pi, 0 <= b,c <= 2*pi.
 //
 //  Note: The operator oveload of () follows the convention: 
 //  First index: "i" index of the phi coordinate, range[0,fNPhi)
 //  Second index: "j" index of theta coordinate, range[0,fNTheta)
 //  Third index: "k" index of chi coordinte, range[0,fNQui)
 //                                                 
 ///////////////////////////////////////////////////////////////////////


//______________________________________________________________
void TSkyMap::Fill(const TF1& f)
{
   // f is used to plot the function

   for (Int_t i = 0; i < fNPhi; ++i)
      for (Int_t j = 0; j < fNPhi; ++j)
         fArray[Coordinate(i,j)] = f.Eval(i*2*Pi()/fNPhi,(2*j+1)*Pi()/fNTheta);
}

//________________________________________________________________
void TSkyMap::Copy(TObject& rhs) const
{
   // Copy this Alm to new one

   TCoeffInfo& info = dynamic_cast<TCoeffInfo&>(rhs);
   TArrayD& arr = dynamic_cast<TArrayD&>(rhs);
   TVMap::Copy(rhs);
   TCoeffInfo::Copy(info);
   TArrayD::Copy(arr);
}

void TSkyMap::Filter(Double_t factor)
{
   // Have to be implemented
}

void TSkyMap::FindSources(Int_t nsources,Double_t r) const
{
   // Print Euler angles of n sources. Uses sort to find biggest values.
   // All coefficients that are withing r(in degrees) 
   // will be considered to belong to the same source and will not be printed.

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

   std::cout << "i) - (phi,theta,gamma) = (Abs(val)) ----- (phi,theta,gamma)\n\n"; 
   std::cout.setf(ios::fixed,ios::floatfield);
   std::cout.precision(5);
   Int_t i = nsources + 1;
   while (--i && (middle != tail)) {
      std::partial_sort(head, ++middle, tail, CompareAbs() );

      std::cout << (nsources - i) << ") - ( " << (head->GetPhiIndex()*360./fNPhi - 180.) 
		<< " , " << (head->GetThetaIndex()*360./fNTheta - 90.) 
	        << " , " << (head->GetQuiIndex()*360./fNQui) << " ) = " 
	        << Abs(head->GetVal()) << " ----- "
	        << "( " << head->GetPhiIndex() << " , " << head->GetThetaIndex() << " , " 
	        << head->GetQuiIndex() << " ) \n";

      tmp.SetPhiTheta(head->GetPhiIndex(),head->GetThetaIndex());
      tail = std::remove_if(middle,tail,tmp);
      ++head;
   }
}

//_________________________________________________
void TSkyMap::CreateAlm(TAlm& alm) const
{
   // Performs a spherical harmonic transform.

   TSphHarmB back(fJ);
   back.SetPoints(*this);
   back.Transform();
   back.GetAlm(alm);
}

void TSkyMap::Draw(Option_t* option)
{
   // Draws the map.

   TH2D* h = CreateHist(0);
   h->Draw(option);
}

//_________________________________________________
TH2D* TSkyMap::CreateHist(Int_t) const
{
   // Creates Histogram

   TH2D* h = new TH2D("phi_theta","histogram",fL,0,2*Pi(),fL,TMath::Pi()/(4*fL),TMath::Pi() + 1./(4*fL));

   Double_t xcon = 2*Pi()/fNPhi;
   Double_t ycon = 2*Pi()/fNTheta;

   for (Int_t u = 0; u < fNPhi; ++u)
      for (Int_t m = 0; m < fNPhi; ++m){
	 Int_t bin = h->FindBin(xcon*m,ycon*u);
	 h->SetBinContent(bin,fArray[Coordinate(m,u,0)]);
      }

   const double min = std::abs(h->GetBinContent(h->GetMinimumBin()));

   for (int i = 0; i < h->GetSize(); ++i)
     (*h)[i] += min;

   //h->GetXaxis()->SetLimits(-180,180);
   //h->GetYaxis()->SetLimits(-90,90);

   return h;
}

