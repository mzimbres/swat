
void coeff2alm(Int_t J)
{
   // Some spherical harmonis are provided to test the algorithm
   // just ancomment the one you want to test.

   // Y_00
   //TF2 f("","1/(2*sqrt(TMath::Pi()))*((x*0 +1)*(y*0 +1))"); 

   // Y_10
   //TF2 f("","(sqrt(3/(TMath::Pi()))*cos(y)/2)*(x*0+1)");

   // first row is to test real alm and second to test pure imaginary alm.

   // Y_11
   //TF2 f("","-2*sqrt(3/(8*TMath::Pi()))*sin(y)*cos(x)"); 
   //TF2 f("","2*sqrt(3/(8*TMath::Pi()))*sin(y)*sin(x)"); 

   // Y_20
   //TF2 f("","sqrt(5/(16*TMath::Pi()))*(3*cos(y)*cos(y)-1)*(x*0 +1)");

   // Y_21
   //TF2 f("","-2*sqrt(15/(8*TMath::Pi()))*sin(y)*cos(y)*cos(x)");
   //TF2 f("","2*sqrt(15/(8*TMath::Pi()))*sin(y)*cos(y)*sin(x)");

   // Y_22
   //TF2 f("","2*sqrt(15/(32*TMath::Pi()))*sin(y)*sin(y)*cos(2*x)");
   //TF2 f("","-2*sqrt(15/(32*TMath::Pi()))*sin(y)*sin(y)*sin(2*x)");

   // Y_30
   //TF2 f("","sqrt(7/(16*TMath::Pi()))*(5*pow(cos(y),3)-3*cos(y))*(x*0+1)");

   // Y_31
   //TF2 f("","-2*sqrt(21/(64*TMath::Pi()))*(5*pow(cos(y),2)-1)*sin(y)*cos(x)");
   //TF2 f("","2*sqrt(21/(64*TMath::Pi()))*(5*pow(cos(y),2)-1)*sin(y)*sin(x)");
   
   // Y_32
   //TF2 f("","2*sqrt(105/(32*TMath::Pi()))*pow(sin(y),2)*cos(y)*cos(2*x)");
   //TF2 f("","-2*sqrt(105/(32*TMath::Pi()))*pow(sin(y),2)*cos(y)*sin(2*x)");

   // Y_33 correto
   //TF2 f("","-2*sqrt(35/(64*TMath::Pi()))*pow(sin(y),3)*cos(3*x)");
   TF2 f("","2*sqrt(35/(64*TMath::Pi()))*pow(sin(y),3)*sin(3*x)");

   TWavCoeff coeff1(J,f);
   TAlm* alm = TAuxFunc::coeff2alm(coeff1);

   Double_t rei,imi;

   for(Int_t l = 0; l < alm->GetL(); ++l){
      for(Int_t m = 0;m <= l; ++m){
	 alm->GetPointComplex(l,m,rei,imi);
	 if (TMath::Abs(rei) < 1e-14) rei = 0;
	 if (TMath::Abs(imi) < 1e-14) imi = 0;
	 cout << "(" << l << "," << m << ") = " << rei <<  " - " << imi << endl;
      }
   }
}


