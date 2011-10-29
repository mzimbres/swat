{
   gROOT->Reset();

   // Will draw a graph of wavelet scale function.

   // Scale function

   TSFunc f(10);
   vector<Double_t> a = f.GetAtScale(0);
   vector<Double_t> bb;
   
   Int_t size = a.size();

   for(Int_t i=0;i<size;++i) 
      bb.push_back(i);

   TGraph *sf0 = new TGraph(size,&bb[0],&a[0]);
   sf0->SetLineColor(kAzure-7);
   sf0->SetFillColor(kAzure-7);
   sf0->SetLineWidth(3);
 
   vector<Double_t> a0 = f.GetAtScale(1);

   TGraph *dk0 = new TGraph(size,&bb[0],&a0[0]);
   dk0->SetLineColor(kRed-7);
   dk0->SetFillColor(kRed-7);
   dk0->SetLineWidth(3);

   vector<Double_t> a1 = f.GetAtScale(2);

   TGraph *dk1 = new TGraph(size,&bb[0],&a1[0]);
   dk1->SetLineColor(kAzure-7);
   dk1->SetFillColor(kAzure-7);
   dk1->SetLineWidth(3);

   vector<Double_t> a2 = f.GetAtScale(3);

   TGraph *dk2 = new TGraph(size,&bb[0],&a2[0]);
   dk2->SetLineColor(kOrange);
   dk2->SetFillColor(kOrange);
   dk2->SetLineWidth(3);

   
   vector<Double_t> a3 = f.GetAtScale(4);

   TGraph *dk3 = new TGraph(size,&bb[0],&a3[0]);
   dk3->SetLineColor(kBlue);
   dk3->SetFillColor(kBlue);
   dk3->SetLineWidth(3);
    
   
   vector<Double_t> a4 = f.GetAtScale(5);

   TGraph *dk4 = new TGraph(size,&bb[0],&a4[0]);
   dk4->SetLineColor(kSpring+3);
   dk4->SetFillColor(kSpring+3);
   dk4->SetLineWidth(3);
    
   //________________

   TMultiGraph *gr = new TMultiGraph();
   gr->Add(sf0);
   gr->Add(dk0);
   gr->Add(dk1);
   gr->Add(dk2);
   gr->Add(dk3);
   gr->Add(dk4);

   TLegend *leg1= new TLegend(0.72,0.55,0.80,0.7);
   leg1->AddEntry(sf0,"j = 0","f");
   leg1->AddEntry(dk0,"j = 1","f");
   leg1->AddEntry(dk1,"j = 2","f");
   leg1->AddEntry(dk2,"j = 3","f");
   leg1->AddEntry(dk3,"j = 4","f");
   leg1->AddEntry(dk4,"j = 5","f");
   leg1->SetShadowColor(kWhite);
   leg1->SetFillColor(kWhite);
   leg1->SetLineColor(kWhite);
 
   TCanvas *canvas = new TCanvas("scale","Scale",200,300,960,400);
   canvas->SetFrameBorderMode(0);
   gr->SetTitle("Scale function at various scales, J = 10");
   gr->Draw("AC");
   gr->GetXaxis()->SetTitle("l");
   gr->GetXaxis()->CenterTitle();
   leg1->Draw();

}

