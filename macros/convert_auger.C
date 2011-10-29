{
   gROOT->Reset();
   TFile *f = new TFile("auger.root","RECREATE");
   TTree *t = new TTree("auger","Public Auger Data");
   Long64_t  nlines = t->ReadFile("auger.dat","ID/I:NSS/I:T/D:P/D:E/D:Time/I:LON/D:LAT/D");
   cout << "Found " << nlines << endl;
   t->Write();

}
