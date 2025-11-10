// compareHists.C
// ROOT macro to overlay all TH1F histograms from two ROOT files
// and produce ratio plots (file2/file1) with 6 plots per PDF page.

#include "TFile.h"
#include "TH1F.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TCollection.h"
#include "TList.h"
#include "TLine.h"

void compareHists(const char* file1name="rthists_10_06_00.root",
                  const char* file2name="rthists_10_12_00.root",
                  const char* outname="comparison.pdf")
{
    gStyle->SetOptStat(0);

    // Open input ROOT files
    TFile *f1 = TFile::Open(file1name);
    TFile *f2 = TFile::Open(file2name);
    if (!f1 || f1->IsZombie() || !f2 || f2->IsZombie()) {
        printf("Error: could not open input files.\n");
        return;
    }

    // Prepare PDF
    TCanvas *c = new TCanvas("c","Comparison",1600,900);
    c->Print(Form("%s[",outname));  // open PDF

    int nplots = 0;

    // Loop over keys in file1
    TIter nextkey(f1->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)nextkey())) {
        TObject *obj = key->ReadObj();
        if (!obj->InheritsFrom("TH1F")) continue;  // only TH1F
        TH1F *h1 = (TH1F*)obj;

        // Find matching histogram in second file
        TH1F *h2 = (TH1F*)f2->Get(h1->GetName());
        if (!h2) {
            printf("Warning: histogram %s not found in second file.\n", h1->GetName());
            continue;
        }

	// skip the empties
	if (h1->Integral() == 0 && h2->Integral() == 0) {
	  continue;
	}
	
        // Create overlay + ratio pads
        if (nplots % 6 == 0) {
            c->Clear();
            c->Divide(3,2); // 6 per page
        }

        c->cd(nplots % 6 + 1);
        TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1.0);
        pad1->SetBottomMargin(0.02);
	pad1->SetLogy();
        pad1->Draw();
        pad1->cd();

        // Draw overlay
        h1->SetLineColor(kBlue);
        h1->SetLineWidth(1);
        h2->SetLineColor(kRed);
        h2->SetLineWidth(1);

        double maxY = std::max(h1->GetMaximum(), h2->GetMaximum());
        //h1->SetMaximum(maxY * 1.3);
        h1->SetMaximum(maxY * 10.0); //for log scale

        h1->Draw("HIST");
        h2->Draw("E0 SAME");

        TLegend *leg = new TLegend(0.35,0.75,0.89,0.89);
        leg->AddEntry(h1, Form("%s", file1name), "l");
        leg->AddEntry(h2, Form("%s", file2name), "l");
        leg->SetBorderSize(0);
        leg->SetTextSize(0.03);
        leg->Draw();

        // Ratio pad
        c->cd(nplots % 6 + 1);
        TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
        pad2->SetTopMargin(0.02);
        pad2->SetBottomMargin(0.3);
        pad2->Draw();
        pad2->cd();

        TH1F *h_ratio = (TH1F*)h2->Clone(Form("ratio_%s",h1->GetName()));
        h_ratio->Divide(h1);
        h_ratio->SetLineColor(kBlack);
        h_ratio->SetMarkerStyle(20);
        h_ratio->SetTitle("");
        h_ratio->GetYaxis()->SetTitle("Ratio (file2/file1)");
        h_ratio->GetYaxis()->SetTitleSize(0.08);
        h_ratio->GetYaxis()->SetTitleOffset(0.5);
        h_ratio->GetYaxis()->SetLabelSize(0.07);
        h_ratio->GetXaxis()->SetLabelSize(0.08);
        h_ratio->GetXaxis()->SetTitleSize(0.1);
        h_ratio->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
        h_ratio->SetMinimum(0.5);
        h_ratio->SetMaximum(1.5);
	h_ratio->SetMarkerSize(0.1);
        h_ratio->Draw("EP");

        TLine *line = new TLine(h_ratio->GetXaxis()->GetXmin(),1,
                                h_ratio->GetXaxis()->GetXmax(),1);
        line->SetLineStyle(2);
        line->Draw("SAME");

        nplots++;
        // Print every 4th plot to a new PDF page
        if (nplots % 6 == 0) {
            c->Print(outname);
        }
    }

    // Print remaining plots if fewer than 6 on last page
    if (nplots % 6 != 0) c->Print(outname);

    // Close PDF
    c->Print(Form("%s]",outname));

    printf("Comparison plots written to %s\n", outname);

    f1->Close();
    f2->Close();
}
