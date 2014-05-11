//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 17 16:00:39 2011 by ROOT version 5.27/04
// from TTree herald/Data from herald file
// found on file: herald.root
//////////////////////////////////////////////////////////

#ifndef TCorrSelector_h
#define TCorrSelector_h

#include <vector>
#include <fstream>

#include <TROOT.h>
#include <TChain.h>
#include <TGraphErrors.h>
#include <TString.h>

#include "TEulerAngle.h"
#include "TAngDistance.h"
#include "TStripeAnalysis.h"

class TCorrSelector : public TStripeAnalysis {
public :
   Int_t                  J;
   Int_t                  fCountCalls;
   TAngDistance*          fAngle;
   Double_t               fAngSep;
   Double_t               fError;
   std::vector<Double_t>  fEnergy;
   std::vector<Double_t>  fEnergyError;
   std::vector<Double_t>  fAngle2;
   std::vector<Double_t>  fAngle2Error;
   TGraphErrors*          fGraph;
   void SetEulerAngles(const TEulerAngle* ang);
   void SetLengthWidth(Double_t l,Double_t w);
   TString fGraphName;
   void SetGraphName(const TString& str);
   void ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c);
   std::ofstream   out;
   std::ofstream   corr;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   ULong64_t       one;
   Int_t           two;
   Double_t        three;
   Double_t        four;
   Double_t        five;
   Double_t        six;
   Double_t        seven;
   Double_t        eight;
   Double_t        nine;
   Double_t        ten;
   Double_t        eleven;
   Double_t        twelve;
   Double_t        thirteen;
   Double_t        fourteen;
   Double_t        fifteen;
   Double_t        sixteen;
   Double_t        seventeen;
   Double_t        eighteen;
   Double_t        nineteen;
   Double_t        twenty;
   Double_t        twentyone;
   Double_t        twentytwo;
   Double_t        twentythree;
   Double_t        twentyfour;
   Double_t        twentyfive;
   Double_t        twentysix;
   Double_t        twentyseven;
   Double_t        twentyeight;
   Double_t        twentynine;
   Double_t        thirty;
   Double_t        thirtyone;
   Double_t        thirtytwo;
   Double_t        thirtythree;
   Double_t        thirtyfour;
   Double_t        thirtyfive;
   Double_t        thirtysix;
   Double_t        thirtyseven;
   Double_t        thirtyeight;
   Double_t        thirtynine;
   Double_t        fourty;
   Double_t        fourtyone;
   Double_t        fourtytwo;

   // List of branches
   TBranch        *b_one;   //!
   TBranch        *b_two;   //!
   TBranch        *b_three;   //!
   TBranch        *b_four;   //!
   TBranch        *b_five;   //!
   TBranch        *b_six;   //!
   TBranch        *b_seven;   //!
   TBranch        *b_eight;   //!
   TBranch        *b_nine;   //!
   TBranch        *b_ten;   //!
   TBranch        *b_eleven;   //!
   TBranch        *b_twelve;   //!
   TBranch        *b_thirteen;   //!
   TBranch        *b_fourteen;   //!
   TBranch        *b_fifteen;   //!
   TBranch        *b_sixteen;   //!
   TBranch        *b_seventeen;   //!
   TBranch        *b_eighteen;   //!
   TBranch        *b_nineteen;   //!
   TBranch        *b_twenty;   //!
   TBranch        *b_twentyone;   //!
   TBranch        *b_twentytwo;   //!
   TBranch        *b_twentythree;   //!
   TBranch        *b_twentyfour;   //!
   TBranch        *b_twentyfive;   //!
   TBranch        *b_twentysix;   //!
   TBranch        *b_twentyseven;   //!
   TBranch        *b_twentyeight;   //!
   TBranch        *b_twentynine;   //!
   TBranch        *b_thirty;   //!
   TBranch        *b_thirtyone;   //!
   TBranch        *b_thirtytwo;   //!
   TBranch        *b_thirtythree;   //!
   TBranch        *b_thirtyfour;   //!
   TBranch        *b_thirtyfive;   //!
   TBranch        *b_thirtysix;   //!
   TBranch        *b_thirtyseven;   //!
   TBranch        *b_thirtyeight;   //!
   TBranch        *b_thirtynine;   //!
   TBranch        *b_fourty;   //!
   TBranch        *b_fourtyone;   //!
   TBranch        *b_fourtytwo;   //!

   TCorrSelector(TTree * /*tree*/ =0): fCountCalls(0)  { }
   virtual ~TCorrSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(TCorrSelector,0); // Selector to calculate deflection vs. 1/E graphs.
};

#endif

#ifdef TCorrSelector_cxx
void TCorrSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("one", &one, &b_one);
   fChain->SetBranchAddress("two", &two, &b_two);
   fChain->SetBranchAddress("three", &three, &b_three);
   fChain->SetBranchAddress("four", &four, &b_four);
   fChain->SetBranchAddress("five", &five, &b_five);
   fChain->SetBranchAddress("six", &six, &b_six);
   fChain->SetBranchAddress("seven", &seven, &b_seven);
   fChain->SetBranchAddress("eight", &eight, &b_eight);
   fChain->SetBranchAddress("nine", &nine, &b_nine);
   fChain->SetBranchAddress("ten", &ten, &b_ten);
   fChain->SetBranchAddress("eleven", &eleven, &b_eleven);
   fChain->SetBranchAddress("twelve", &twelve, &b_twelve);
   fChain->SetBranchAddress("thirteen", &thirteen, &b_thirteen);
   fChain->SetBranchAddress("fourteen", &fourteen, &b_fourteen);
   fChain->SetBranchAddress("fifteen", &fifteen, &b_fifteen);
   fChain->SetBranchAddress("sixteen", &sixteen, &b_sixteen);
   fChain->SetBranchAddress("seventeen", &seventeen, &b_seventeen);
   fChain->SetBranchAddress("eighteen", &eighteen, &b_eighteen);
   fChain->SetBranchAddress("nineteen", &nineteen, &b_nineteen);
   fChain->SetBranchAddress("twenty", &twenty, &b_twenty);
   fChain->SetBranchAddress("twentyone", &twentyone, &b_twentyone);
   fChain->SetBranchAddress("twentytwo", &twentytwo, &b_twentytwo);
   fChain->SetBranchAddress("twentythree", &twentythree, &b_twentythree);
   fChain->SetBranchAddress("twentyfour", &twentyfour, &b_twentyfour);
   fChain->SetBranchAddress("twentyfive", &twentyfive, &b_twentyfive);
   fChain->SetBranchAddress("twentysix", &twentysix, &b_twentysix);
   fChain->SetBranchAddress("twentyseven", &twentyseven, &b_twentyseven);
   fChain->SetBranchAddress("twentyeight", &twentyeight, &b_twentyeight);
   fChain->SetBranchAddress("twentynine", &twentynine, &b_twentynine);
   fChain->SetBranchAddress("thirty", &thirty, &b_thirty);
   fChain->SetBranchAddress("thirtyone", &thirtyone, &b_thirtyone);
   fChain->SetBranchAddress("thirtytwo", &thirtytwo, &b_thirtytwo);
   fChain->SetBranchAddress("thirtythree", &thirtythree, &b_thirtythree);
   fChain->SetBranchAddress("thirtyfour", &thirtyfour, &b_thirtyfour);
   fChain->SetBranchAddress("thirtyfive", &thirtyfive, &b_thirtyfive);
   fChain->SetBranchAddress("thirtysix", &thirtysix, &b_thirtysix);
   fChain->SetBranchAddress("thirtyseven", &thirtyseven, &b_thirtyseven);
   fChain->SetBranchAddress("thirtyeight", &thirtyeight, &b_thirtyeight);
   fChain->SetBranchAddress("thirtynine", &thirtynine, &b_thirtynine);
   fChain->SetBranchAddress("fourty", &fourty, &b_fourty);
   fChain->SetBranchAddress("fourtyone", &fourtyone, &b_fourtyone);
   fChain->SetBranchAddress("fourtytwo", &fourtytwo, &b_fourtytwo);
}

Bool_t TCorrSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef TCorrSelector_cxx
