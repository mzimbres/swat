
/***************************************************
 Rafael Alves Batista
 December 10, 2010.
 
 
 This routine draws the latitude and longitude lines 
 in the aitoff projection.
 
 It was adapted from:
 http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=6650
 ***************************************************/


void DrawLatitudesLongitudes(TCanvas *c)
{
	const int nlat=9; // Number of latitudes lines
	const int nlon=9; // Number of longitudes lines
	Int_t M=15;
	
	TGraph *latitudes[nlat];
	TGraph *longitudes[nlon];
	
	TGraph *g1 = new TGraph();  
	TGraph *g2 = new TGraph();  
	
	
	for(Int_t j=0; j<nlat; ++j){
		latitudes[j]=new TGraph();
		Float_t la=-90+180/(nlat-1)*j;
		for(Int_t i=0;i<M+1;++i){
			Float_t lo= -180+360/M*i;
			Float_t z = sqrt(1+cos(la*TMath::DegToRad())*cos(lo*TMath::DegToRad()/2));
			Float_t x = 180*cos(la*TMath::DegToRad())*sin(lo*TMath::DegToRad()/2)/z;
			Float_t y = 90*sin(la*TMath::DegToRad())/z;
			latitudes[j]->SetPoint(i,x,y);
		}
		
	} 
	
	for(Int_t j=0; j<nlon; ++j){
		longitudes[j]=new TGraph();
		Float_t lo=-180+360/(nlon-1)*j;
		for(Int_t i=0;i<M+1;++i){
			Float_t la= -90+180/M*i;
			Float_t z = sqrt(1+cos(la*TMath::DegToRad())*cos(lo*TMath::DegToRad()/2));
			Float_t x = 180*cos(la*TMath::DegToRad())*sin(lo*TMath::DegToRad()/2)/z;
			Float_t y = 90*sin(la*TMath::DegToRad())/z;
			longitudes[j]->SetPoint(i,x,y);
				//cout << x << " " << y << " " << z << endl;
		}
	}
	
	
		// Draw the grid	
	TPad *pad2 = new TPad("pad2","",0,0,1,1);
	pad2->SetFillStyle(4000);
	pad2->SetFillColor(0);
	pad2->SetBorderSize(0);
	pad2->SetFrameBorderMode(0);
	pad2->SetFrameLineColor(0); 
	pad2->SetFrameBorderMode(0);
	pad2->Draw();
	pad2->cd();
	Double_t ymin = -89.5;
	Double_t ymax = 89.5;
	Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
	Double_t xmin = -180;
	Double_t xmax = 180;
	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
	
	pad2->SetTicky(false);
	pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
	gStyle->SetPadBorderMode(0);
	
	for(Int_t j=0;j<nlat;++j) latitudes[j]->Draw("L");
	for(Int_t j=0;j<nlon;++j) longitudes[j]->Draw("L"); 
	
}
