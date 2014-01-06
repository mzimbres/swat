{
   // This macro converts a Herald ascii data file to a TTree.

   gROOT->Reset();
   TFile *f = new TFile("herald.root","RECREATE");
   TTree *t = new TTree("events","Data from herald file");
   Long64_t  nlines = t->ReadFile("herald.dat","one/l:two/I:three/D:four/D:five/D:six/D:seven/D:eight:nine:ten:eleven:twelve:thirteen:fourteen:fifteen:sixteen:seventeen:eighteen:nineteen:twenty:twentyone:twentytwo:twentythree:twentyfour:twentyfive:twentysix:twentyseven:twentyeight:twentynine:thirty:thirtyone:thirtytwo:thirtythree:thirtyfour:thirtyfive:thirtysix:thirtyseven:thirtyeight:thirtynine:fourty:fourtyone:fourtytwo");
   cout << "Found " << nlines << endl;
   t->Write();
}
