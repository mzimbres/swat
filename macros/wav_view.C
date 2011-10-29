
void wav_view(Int_t J,Int_t j,Int_t N,Int_t alpha,Int_t beta,Int_t gamma)
{
   // Draws wavelets using backward transform.
   // j: wavelet at scale j
   // J: maximum scale
   // (m,u,n) wavelet position.
   
   TWavMap wav(j,J,N);
   wav(alpha,beta,gamma) = 1;

   TAlm alm(J);
   wav.CreateAlm(alm);

   TSkyMap* sky = alm.SHT();

   TH2D *h1 = sky->CreateHist(gamma);
   delete sky;

   h1->SetName("wavelet");
   h1->SetTitle("");
   h1->GetXaxis()->SetAxisColor(kWhite);
   h1->GetXaxis()->SetLabelColor(kWhite);
   h1->GetYaxis()->SetAxisColor(kWhite);
   h1->GetYaxis()->SetLabelColor(kWhite);
   TCanvas *c1 = new TCanvas("c1","Wavelet",500,100,450,450);
   h1->Draw("aitoff");
}

void wav_view_all(Int_t J,Int_t j,Int_t N)
{
   // Draws wavelets using backward transform.
   // j: wavelet at scale j
   // J: maximum scale
   // (m,u,n) wavelet position.
   
   TWavMap wav(j,J,N);
   wav(source0->AlphaInd(),source0->BetaInd(),source0->GammaInd()) = 1;
   wav(source1->AlphaInd(),source1->BetaInd(),source1->GammaInd()) = 1;
   wav(source2->AlphaInd(),source2->BetaInd(),source2->GammaInd()) = 1;
   wav(source3->AlphaInd(),source3->BetaInd(),source3->GammaInd()) = 1;
   wav(source4->AlphaInd(),source4->BetaInd(),source4->GammaInd()) = 1;
   wav(source5->AlphaInd(),source5->BetaInd(),source5->GammaInd()) = 1;
   wav(source6->AlphaInd(),source6->BetaInd(),source5->GammaInd()) = 1;

   TAlm alm(J);
   wav.CreateAlm(alm);

   TSkyMap* sky = alm.SHT();

   TH2D *h1 = sky->CreateHist(0);
   delete sky;

   h1->SetName("wavelet");
   h1->SetTitle("wavelet");
   TCanvas *c1 = new TCanvas("c1","Wavelet",500,100,450,450);
   h1->Draw("aitoff");
}
