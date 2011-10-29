
void sph_harm(Int_t l, Int_t m)
{
   // You can use this function to draw 
   // some spherical harmonics.
   
   Int_t L = 50;
   if(l>50) L = l+1;

   // In case you want draw spherical harmonics with l >= 2^8
   // change the constructor below.
   TAlm aa(8);
   aa.SetPoint(l,m,1,0);

   TSkyMap* map = aa.SHT();

   TH2D* h = map->CreateHist(0);
   delete map;
   h->SetName("Sphericalharmonic");
   h->SetTitle("");
   h->GetXaxis()->SetAxisColor(kWhite);
   h->GetXaxis()->SetLabelColor(kWhite);
   h->GetYaxis()->SetAxisColor(kWhite);
   h->GetYaxis()->SetLabelColor(kWhite);

   TCanvas *c3 = new TCanvas("c3","Kernel",1010,100,460,460);
   c3->GetFrame()->SetLineColor(kWhite);
   c3->GetFrame()->SetBorderSize(0);
   
   h->Draw("aitoff");

}

