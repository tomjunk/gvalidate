//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 27 15:37:04 2017 by ROOT version 6.10/04
// from TTree gRooTracker/GENIE event tree rootracker format
// found on file: test2.root
//////////////////////////////////////////////////////////

#ifndef rootracker_h
#define rootracker_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

// Header file for the classes stored in the TTree if any.
#include "TBits.h"
#include "TObjString.h"

class rootracker {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //TBits           *EvtFlags;
   UInt_t          fNbits;
   UInt_t          fNbytes;
   UChar_t         fAllBits[1];   //[fNbytes]
 //TObjString      *EvtCode;
   TString         fString;
   Int_t           EvtNum;
   Double_t        EvtXSec;
   Double_t        EvtDXSec;
   Double_t        EvtWght;
   Double_t        EvtProb;
   Double_t        EvtVtx[4];
   Int_t           StdHepN;
   Int_t           StdHepPdg[54];   //[StdHepN]
   Int_t           StdHepStatus[54];   //[StdHepN]
   Int_t           StdHepRescat[54];   //[StdHepN]
   Double_t        StdHepX4[54][4];   //[StdHepN]
   Double_t        StdHepP4[54][4];   //[StdHepN]
   Double_t        StdHepPolz[54][3];   //[StdHepN]
   Int_t           StdHepFd[54];   //[StdHepN]
   Int_t           StdHepLd[54];   //[StdHepN]
   Int_t           StdHepFm[54];   //[StdHepN]
   Int_t           StdHepLm[54];   //[StdHepN]

   // List of branches
   TBranch        *b_EvtFlags_fNbits;   //!
   TBranch        *b_EvtFlags_fNbytes;   //!
   TBranch        *b_fAllBits;   //!
   TBranch        *b_EvtCode_fString;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_EvtXSec;   //!
   TBranch        *b_EvtDXSec;   //!
   TBranch        *b_EvtWght;   //!
   TBranch        *b_EvtProb;   //!
   TBranch        *b_EvtVtx;   //!
   TBranch        *b_StdHepN;   //!
   TBranch        *b_StdHepPdg;   //!
   TBranch        *b_StdHepStatus;   //!
   TBranch        *b_StdHepRescat;   //!
   TBranch        *b_StdHepX4;   //!
   TBranch        *b_StdHepP4;   //!
   TBranch        *b_StdHepPolz;   //!
   TBranch        *b_StdHepFd;   //!
   TBranch        *b_StdHepLd;   //!
   TBranch        *b_StdHepFm;   //!
   TBranch        *b_StdHepLm;   //!

   rootracker(TString filename="rootracker_test.root");
   virtual ~rootracker();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double eneutrinomc=4.0, TString outputfilename="rthists.root");
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef rootracker_cxx
rootracker::rootracker(TString filename) : fChain(0) 
{
  TTree *tree = 0;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
      if (!f || !f->IsOpen()) {
         f = new TFile(filename);
      }
      f->GetObject("gRooTracker",tree);

   }
   Init(tree);
}

rootracker::~rootracker()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t rootracker::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t rootracker::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void rootracker::Init(TTree *tree)
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
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fNbits", &fNbits, &b_EvtFlags_fNbits);
   fChain->SetBranchAddress("fNbytes", &fNbytes, &b_EvtFlags_fNbytes);
   fChain->SetBranchAddress("fAllBits", &fAllBits, &b_fAllBits);
   fChain->SetBranchAddress("fString", &fString, &b_EvtCode_fString);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("EvtXSec", &EvtXSec, &b_EvtXSec);
   fChain->SetBranchAddress("EvtDXSec", &EvtDXSec, &b_EvtDXSec);
   fChain->SetBranchAddress("EvtWght", &EvtWght, &b_EvtWght);
   fChain->SetBranchAddress("EvtProb", &EvtProb, &b_EvtProb);
   fChain->SetBranchAddress("EvtVtx", EvtVtx, &b_EvtVtx);
   fChain->SetBranchAddress("StdHepN", &StdHepN, &b_StdHepN);
   fChain->SetBranchAddress("StdHepPdg", StdHepPdg, &b_StdHepPdg);
   fChain->SetBranchAddress("StdHepStatus", StdHepStatus, &b_StdHepStatus);
   fChain->SetBranchAddress("StdHepRescat", StdHepRescat, &b_StdHepRescat);
   fChain->SetBranchAddress("StdHepX4", StdHepX4, &b_StdHepX4);
   fChain->SetBranchAddress("StdHepP4", StdHepP4, &b_StdHepP4);
   fChain->SetBranchAddress("StdHepPolz", StdHepPolz, &b_StdHepPolz);
   fChain->SetBranchAddress("StdHepFd", StdHepFd, &b_StdHepFd);
   fChain->SetBranchAddress("StdHepLd", StdHepLd, &b_StdHepLd);
   fChain->SetBranchAddress("StdHepFm", StdHepFm, &b_StdHepFm);
   fChain->SetBranchAddress("StdHepLm", StdHepLm, &b_StdHepLm);
   Notify();
}

Bool_t rootracker::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void rootracker::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry,1000);
}
Int_t rootracker::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef rootracker_cxx
