{
   gROOT->Reset();

   // Will draw a graph of wavelet kernels

   // Total number of scales 
   const Int_t J = 10;

   // Discrete kernel at scale j = 0

   TDKernel f0(0,J);
   TArrayD a0 = f0.GetKernel();
   
   Int_t size = f0.GetLmax();
   TArrayD b(size);

   for(Int_t i=0;i<size;++i) 
      b[i] = i;

   TGraph *dk0 = new TGraph(size,b.GetArray(),a0.GetArray());
   dk0->SetLineColor(kRed-7);
   dk0->SetFillColor(kRed-7);
   dk0->SetLineWidth(3);

   // Discrete kernel at scale j = 1
   
   TDKernel f1(1,J);
   TArrayD a1 = f1.GetKernel();

   TGraph *dk1 = new TGraph(size,b.GetArray(),a1.GetArray());
   dk1->SetLineColor(kAzure-7);
   dk1->SetFillColor(kAzure-7);
   dk1->SetLineWidth(3);

   // Discrete kernel at scale j = 2
   
   TDKernel f2(2,J);
   TArrayD a2 = f2.GetKernel();

   TGraph *dk2 = new TGraph(size,b.GetArray(),a2.GetArray());
   dk2->SetLineColor(kOrange);
   dk2->SetFillColor(kOrange);
   dk2->SetLineWidth(3);

   // Discrete kernel at scale j = 3
   
   TDKernel f3(3,J);
   TArrayD a3 = f3.GetKernel();

   TGraph *dk3 = new TGraph(size,b.GetArray(),a3.GetArray());
   dk3->SetLineColor(kBlue);
   dk3->SetFillColor(kBlue);
   dk3->SetLineWidth(3);
    
   // Discrete kernel at scale j = 4
   
   TDKernel f4(4,J);
   TArrayD a4 = f4.GetKernel();

   TGraph *dk4 = new TGraph(size,b.GetArray(),a4.GetArray());
   dk4->SetLineColor(kSpring+3);
   dk4->SetFillColor(kSpring+3);
   dk4->SetLineWidth(3);
    
   //________________

   TMultiGraph *gr = new TMultiGraph();
   gr->Add(dk0);
   gr->Add(dk1);
   gr->Add(dk2);
   gr->Add(dk3);
   gr->Add(dk4);

   TLegend *leg1= new TLegend(0.72,0.55,0.80,0.7);
   leg1->AddEntry(dk0,"j = 0","f");
   leg1->AddEntry(dk1,"j = 1","f");
   leg1->AddEntry(dk2,"j = 2","f");
   leg1->AddEntry(dk3,"j = 3","f");
   leg1->AddEntry(dk4,"j = 4","f");
   leg1->SetShadowColor(kWhite);
   leg1->SetFillColor(kWhite);
   leg1->SetLineColor(kWhite);
 
   TCanvas *canvas = new TCanvas("c2","Kernel",500,600,960,400);
   canvas->SetFrameBorderMode(0);
   gr->SetTitle("Kernel at various scales, J = 10");
   gr->Draw("AC");
   gr->GetXaxis()->SetTitle("l");
   gr->GetYaxis()->SetTitle("Kernel values");
   gr->GetXaxis()->CenterTitle();
   gr->GetYaxis()->CenterTitle();
   leg1->Draw();

}

