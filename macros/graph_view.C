
void two_graphs(const TObject* g1,const TObject* g2)
{

   TCanvas *canvas = new TCanvas("Canvas","Aitoff",200,10,700,780);
   canvas->Divide(2);
   canvas->cd(1);
   g1->Draw("AP");
   canvas->cd(2);
   g2->Draw("AP");
}

void three_graphs(const TObject* g1,const TObject* g2,const TObject* g3)
{

   TCanvas *three = new TCanvas("three","Three graphs",200,10,700,780);
   canvas->Divide(3);
   canvas->cd(1);
   g1->Draw("AP");
   canvas->cd(2);
   g2->Draw("AP");
   canvas->cd(3);
   g3->Draw("AP");
}

void six_graphs()
{
   TCanvas *six = new TCanvas("six","Six graphs",200,10,700,780);
   six->Divide(2,3);
   six->cd(1);
   g0->Draw("AP");
   six->cd(2);
   g1->Draw("AP");
   six->cd(3);
   g2->Draw("AP");
   six->cd(4);
   g3->Draw("AP");
   six->cd(5);
   g4->Draw("AP");
   six->cd(6);
   g5->Draw("AP");
}
