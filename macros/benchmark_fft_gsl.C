// Author: Marcelo Zimbres Silva, 2011.

#include <vector>
#include <iostream>

// ROOT

#include "Math/SpecFunc.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TBenchmark.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLegend.h"

// SWAT

#include "TWignerd.h"
#include "TSmallWignerd.h"

using namespace TMath;

void benchmark_fft_gsl(Int_t lmax = 150,Int_t L = 1024)
{
   // Benchmark

   if (L <= lmax){
      cerr << "L must be greater than lmax." << endl;
      return;
   }

   Double_t size = 2*L;
   vector<double> cosine(size);
   for (Int_t i = 0; i < size; ++i)
      cosine[i] = Cos(Pi()*(i + 1/2)/size);

   TGraph* gsl = new TGraph(lmax + 1);
   gsl->SetMarkerColor(kBlue);
   gsl->SetLineColor(kBlue);

   // gsl
   vector<double> arr(size);
   Float_t time = 0;
   for (Int_t l = 0; l <= lmax; ++l){
      gBenchmark->Start("recursion");
      for (Int_t m = 0; m <= l; ++m)
         for (Int_t i = 0; i < size; ++i)
            arr[i] = ROOT::Math::assoc_legendre(l,m,cosine[i]);
      gBenchmark->Stop("recursion");
      time += gBenchmark->GetCpuTime("recursion");
      gsl->SetPoint(l,l,time);
      gBenchmark->Reset();
   }

   // Wigner_d
   TGraph* fft = new TGraph(lmax + 1);
   fft->SetMarkerColor(kRed);

   TWignerd wig(lmax + 1);
   TSmallWignerd smallwd(L);

   Double_t *vecy = NULL;
   time = 0;
   for (Int_t l = 0; l <= lmax; ++l){
      gBenchmark->Start("fft");
      for (Int_t m = 0; m <= l; ++m)
         vecy = smallwd.Get(wig,m,0);
      wig.Recurse();
      gBenchmark->Stop("fft");
      time += gBenchmark->GetCpuTime("fft");
      gBenchmark->Reset();
      fft->SetPoint(l,l,time);
   }

   TLegend* leg = new TLegend(0.3,0.6,0.6,0.9);
   leg->SetFillColor(kWhite);
   leg->AddEntry(gsl,"Recursion","p");
   leg->AddEntry(fft,"FFT","p");


   TMultiGraph* g = new TMultiGraph("g","Recursion vs FFT");
   g->Add(gsl);
   g->Add(fft);
   g->Draw("ap");
   leg->Draw();
}

void compare_graphs(Int_t l,Int_t m,Int_t L = 1024)
{
   // Macro can be used to compare graphs of associated legendre
   // polynomials using gsl(recursion) and fft.
   // The size of the signal will be set to 2*L.
   // The graphs should differ only by a normalization factor.

   Double_t pi = TMath::Pi();
   Int_t size = 2*L;

   TF1* f = new TF1("f","ROOT::Math::assoc_legendre([0],[1],cos(x))",0,pi);
   f->SetParameter(0,l);
   f->SetParameter(1,m);
   f->SetNpx(size);
   f->SetTitle("GSL");
   f->SetLineColor(kSpring+3);
   f->SetFillColor(kSpring+3);
   f->SetLineWidth(2);

   TWignerd wig(l);
   wig.Advance(l);

   TSmallWignerd smallwd(L);

   Double_t *vecy = smallwd.Get(wig,m,0);

   Double_t vecx[size];
   for (Int_t j = 0; j < size; ++j) 
      vecx[j] = TMath::Pi()*(j + 1/2)/size;

   TGraph *gr = new TGraph(size,vecx,vecy);
   gr->SetTitle("FFT");
   gr->SetLineColor(kSpring+3);
   gr->SetFillColor(kSpring+3);
   gr->SetLineWidth(2);

   TCanvas *canvas = new TCanvas("Canvas","Aitoff",1000,500);
   canvas->Divide(2);
   canvas->cd(1);
   gr->Draw("ac");
   canvas->cd(2);
   f->Draw();
}

//void compare_values(Int_t lmax = 20,Int_t L = 1024)
//{
//   // This macro will compare values obtained by using fft and recursion
//   // for all l am up to lmax and print the biggest error.
//
//   if (L <= lmax){
//      cerr << "L must be greater than lmax." << endl;
//      return;
//   }
//
//   Double_t size = 2*L;
//
//   vector<double> cosine(size);
//   for (Int_t i = 0; i < size; ++i)
//      cosine[i] = Cos(Pi()*(i + 1/2)/size);
//
//   TWignerd wig(lmax + 1);
//   TSmallWignerd smallwd(L);
//
//   Double_t error = 0.;
//   Double_t *vecy = NULL;
//   vector<Double_t> arr(size);
//
//   for (Int_t l = lmax; l <= lmax; ++l){
//      for (Int_t m = 0; m <= lmax; ++m){
//         Double_t tmp = Sqrt(Factorial(l + m)/Factorial(l - m));
//         vecy = smallwd.Get(wig,m,0);
//         for (Int_t i = 0; i < size; ++i){
//            arr[i] = ROOT::Math::assoc_legendre(l,m,cosine[i])/tmp;
//	    Double_t diff = Abs(arr[i] - vecy[i]);
//	    if (diff > error) error = diff;
//	 }
//      }
//      wig.Recurse();
//   }
//
//   std::cout << "Biggest error: " << error << endl;
//
//}



