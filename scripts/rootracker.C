#define rootracker_cxx
#include "rootracker.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <vector>
#include <algorithm>
#include <TRandom.h>

// eneutrino is used to scale plots

void rootracker::Loop(double eneutrinomc, TString outputfilename)
{
  //   In a ROOT session, you can do:
  //      root> .L rootracker.C
  //      root> rootracker t
  //      root> t.GetEntry(12); // Fill t data members with entry number 12
  //      root> t.Show();       // Show values of entry 12
  //      root> t.Show(16);     // Read and show values of entry 16
  //      root> t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;

  std::vector<int> particlelist{
    11, -11, 12, -12,
    13, -13, 14, -14,
    15, -15, 16, -16,
    22,
    111, 211, -211,
    130, 310, 311, -311, 321, -321,
    2212, -2212, 2112, -2112,
    3122, -3122, 3222, -3222, 3212, -3212, 3112, -3112};
  
  std::vector<TString> particlenames{
    "e-", "e+", "nue", "nuebar",
    "mu-", "mu+", "numu", "numubar",
    "tau-", "tau+", "nutau", "nutaubar",
    "gamma",
    "pi0", "pi+", "pi-",
    "KLong", "KShort", "K0", "K0bar", "K+", "K-",
    "p", "pbar", "n", "nbar",
    "Lambda", "Lambdabar", "Sigma+", "Sigma+bar", "Sigma0", "Sigma0bar", "Sigma-", "Sigma-bar"};

  std::map<int,TH1F*> ppartcc, ppartnc, ptpartcc, ptpartnc, costhetapartcc, costhetapartnc, npartcc, npartnc;
  
  for (size_t i=0; i<particlelist.size(); ++i)
    {
      auto pn=particlenames.at(i);
      auto ipdg = particlelist.at(i);
      TString hname="";
      TString htitle="";
      
      hname="ccpartmom";
      hname += pn;
      htitle=pn;
      htitle += " Momentum (GeV);";
      htitle += htitle;
      htitle += "Particles in CC Interactions";
      ppartcc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,0,eneutrinomc);
      ppartcc[ipdg]->Sumw2();
      
      hname="ncpartmom";
      hname += pn;
      htitle=pn;
      htitle += " Momentum (GeV);";
      htitle += htitle;
      htitle += "Particles in NC Interactions";
      ppartnc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,0,eneutrinomc);
      ppartnc[ipdg]->Sumw2();

      hname="ccpartpt";
      hname += pn;
      htitle=pn;
      htitle += " PT (GeV);";
      htitle += htitle;
      htitle += "Particles in CC Interactions";
      ptpartcc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,0,eneutrinomc);
      ptpartcc[ipdg]->Sumw2();

      hname="ncpartpt";
      hname += pn;
      htitle=pn;
      htitle += " PT (GeV);";
      htitle += htitle;
      htitle += "Particles in NC Interactions";
      ptpartnc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,0,eneutrinomc);
      ptpartnc[ipdg]->Sumw2();

      hname="ccpartcost";
      hname += pn;
      htitle=pn;
      htitle += " Costheta;";
      htitle += htitle;
      htitle += "Particles in CC Interactions";
      costhetapartcc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,-1,1);
      costhetapartcc[ipdg]->Sumw2();

      hname="ncpartcost";
      hname += pn;
      htitle=pn;
      htitle += " Costheta;";
      htitle += htitle;
      htitle += "Particles in NC Interactions";
      costhetapartnc[ipdg] =  (TH1F*) new TH1F(hname,htitle,100,-1,1);
      costhetapartnc[ipdg]->Sumw2();

      hname="ccpartnum";
      hname += pn;
      htitle=pn;
      htitle += " Multiplicity;";
      htitle += htitle;
      htitle += "CC Interactions";
      npartcc[ipdg] =  (TH1F*) new TH1F(hname,htitle,20,-0.5,19.5);
      npartcc[ipdg]->Sumw2();

      hname="ncpartnum";
      hname += pn;
      htitle=pn;
      htitle += " Multiplicity;";
      htitle += htitle;
      htitle += "NC Interactions";
      npartnc[ipdg] =  (TH1F*) new TH1F(hname,htitle,20,-0.5,19.5);
      npartnc[ipdg]->Sumw2();
    }

  TH1F *qsquaredcc = (TH1F*) new TH1F("qsquaredcc"," ;Q^2 (GeV);Events",40,0,eneutrinomc*eneutrinomc);
  qsquaredcc->Sumw2();
  TH1F *qsquarednc = (TH1F*) new TH1F("qsquarednc"," ;Q^2 (GeV);Events",40,0,eneutrinomc*eneutrinomc);
  qsquarednc->Sumw2();
  
  // some unused histograms
  
  TH1F *truecc = (TH1F*) new TH1F("truecc"," ;True Enu (GeV);Events",40,0,eneutrinomc);
  TH1F *truenc = (TH1F*) new TH1F("truenc"," ;True Enu (GeV);Events",40,0,eneutrinomc);
  TH1F *recoecc = (TH1F*) new TH1F("recoecc"," ;Reco Enu vis (GeV);Events",40,0,eneutrinomc);
  TH1F *recoenc = (TH1F*) new TH1F("recoenc"," ;Reco Enu vis (GeV);Events",40,0,eneutrinomc);
  TH1F *eresidcc = (TH1F*) new TH1F("eresidcc"," ;Vis. Frac. Resid.E;Events",80,-1,.2);
  TH1F *eresidnc = (TH1F*) new TH1F("eresidnc"," ;Vis. Frac. Resid.E;Events",80,-1,.2);

  TH1F *recoencc = (TH1F*) new TH1F("recoencc"," ;Reco Enu w/neut (GeV);Events",40,0,eneutrinomc);
  TH1F *recoennc = (TH1F*) new TH1F("recoennc"," ;Reco Enu NC w/neut (GeV);Events",40,0,eneutrinomc);
  TH1F *eresidncc = (TH1F*) new TH1F("eresidncc"," ;frac. Resid.E w/neut;Events",80,-1,.2);
  TH1F *eresidnnc = (TH1F*) new TH1F("eresidnnc"," ;frac. Resid.E w/neut;Events",80,-1,.2);

  TH1F *recoeacc = (TH1F*) new TH1F("recoeacc"," ;Reco Enu ArgoNeut (GeV);Events",40,0,eneutrinomc);
  TH1F *recoeanc = (TH1F*) new TH1F("recoeanc"," ;Reco Enu NC ArgoNeut (GeV);Events",40,0,eneutrinomc);
  TH1F *eresidacc = (TH1F*) new TH1F("eresidacc"," ;frac. Resid.E ArgoNeut;Events",80,-1,.2);
  TH1F *eresidanc = (TH1F*) new TH1F("eresidanc"," ;frac. Resid.E  ArgoNeut;Events",80,-1,.2);

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // look for a lepton of the right sign and flavor to identify CC interactions
    // the nuclear remnant may not be revealing though.
    // the incoming neutrino is assumed to be in the first slot

    bool iscc=false;
    float elepton = 0;
    int neutcode = StdHepPdg[0];
    float eneut = StdHepP4[0][3];
    float enreco = 0;

    float qsquared = 0;
    
    // determine if we are looking at CC or NC
    
    for (int i=1;i<StdHepN;++i)
      {
	int pdg = StdHepPdg[i];
	int apdg = TMath::Abs(pdg);

	int status = StdHepStatus[i];
	if (status != 1) continue;  // 1: simulate, 15: nuclear recoil or blob?

	if ( (pdg == 11 && neutcode == 12) || ( pdg == -11 && neutcode == -12) ||
	     (pdg == 13 && neutcode == 14) || ( pdg == -13 && neutcode == -14) )
	  { 
	    iscc=true;
	    elepton = StdHepP4[i][3];
	    qsquared = TMath::Sq(StdHepP4[i][1]-StdHepP4[0][1]) +
	      TMath::Sq(StdHepP4[i][2]-StdHepP4[0][2]) +
	      TMath::Sq(StdHepP4[i][3]-StdHepP4[0][3]) -
	      TMath::Sq(StdHepP4[i][0]-StdHepP4[0][0]);
	    break;
	  }
	else if ( pdg == neutcode )
	  {
	    qsquared = TMath::Sq(StdHepP4[i][1]-StdHepP4[0][1]) +
	      TMath::Sq(StdHepP4[i][2]-StdHepP4[0][2]) +
	      TMath::Sq(StdHepP4[i][3]-StdHepP4[0][3]) -
	      TMath::Sq(StdHepP4[i][0]-StdHepP4[0][0]);
	    break;
	  }
      }

    if (iscc)
      {
	qsquaredcc->Fill(qsquared);
      }
    else
      {
	qsquarednc->Fill(qsquared);
      }
    
    //std::cout << "diagnostic: " << iscc << " " << elepton << " " << StdHepN << std::endl;

    // particle counts by species
    
    std::map<int,int> pcountcc, pcountnc;	
    for (const auto& ipl : particlelist)
      {
	pcountcc[ipl] = 0;
	pcountnc[ipl] = 0;
      }
    
    for (int i=1;i<StdHepN;++i)
      {
	int pdg = StdHepPdg[i];
	int apdg = TMath::Abs(pdg);

	int status = StdHepStatus[i];
	if (status != 1) continue;  // 1: simulate, 15: nuclear recoil or blob?

	double pmass = StdHepP4[i][3]*StdHepP4[i][3] -
	  StdHepP4[i][0]*StdHepP4[i][0] -
	  StdHepP4[i][1]*StdHepP4[i][1] -
	  StdHepP4[i][2]*StdHepP4[i][2];

	if (pmass>0) pmass = sqrt(pmass);
	else pmass = 0;

	double kinetic_energy = StdHepP4[i][3] - pmass;

	double px = StdHepP4[i][0];
	double py = StdHepP4[i][1];
	double pz = StdHepP4[i][2];
	double pt = TMath::Sqrt(px*px + py*py);
	double ptot = TMath::Sqrt(px*px + py*py + pz*pz);
	double costheta = -2;
	if (ptot>0) costheta = pz/ptot;

	auto htest = ppartcc.find(pdg);
	//std::cout << "   " << pdg << std::endl;
	if (htest != ppartcc.end())
	  {
	    if (iscc)
	      {
		ppartcc[pdg]->Fill(ptot);
		ptpartcc[pdg]->Fill(pt);
		costhetapartcc[pdg]->Fill(costheta);
		if (pcountcc.find(pdg) == pcountcc.end())
		  {
		    pcountcc[pdg] = 0;
		  }
		++pcountcc[pdg];
	      }
	    else
	      {
		ppartnc[pdg]->Fill(ptot);
		ptpartnc[pdg]->Fill(pt);
		costhetapartnc[pdg]->Fill(costheta);
		if (pcountnc.find(pdg) == pcountnc.end())
		  {
		    pcountnc[pdg] = 0;
		  }
		++pcountnc[pdg];
	      }
	  }
	for (const auto& pind : pcountcc)
	  {
	    //std::cout << " num diag: " << pind.first << " " << pind.second << std::endl;
	    npartcc[pind.first]->Fill(pind.second);
	  }
	for (const auto& pind : pcountnc)
	  {
	     npartnc[pind.first]->Fill(pind.second);
	  }
      }
    // if (Cut(ientry) < 0) continue;
  }

  TFile outputrootfile(outputfilename,"RECREATE");

  for (const auto &h : ppartcc)
    {
      h.second->Write();
    }
  for (const auto &h : ppartnc)
    {
      h.second->Write();
    }
  for (const auto &h : ptpartcc)
    {
      h.second->Write();
    }
  for (const auto &h : ptpartnc)
    {
      h.second->Write();
    }
  for (const auto &h : costhetapartcc)
    {
      h.second->Write();
    }
  for (const auto &h : costhetapartnc)
    {
      h.second->Write();
    }
  for (const auto &h : npartcc)
    {
      h.second->Write();
    }
  for (const auto &h : npartnc)
    {
      h.second->Write();
    }
  qsquaredcc->Write();
  qsquarednc->Write();
  
  outputrootfile.Close();

}
