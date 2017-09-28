#define monotop_analysis_cxx

#include "monotop_analysis.h"
#include <TH2.h>
#include <TStyle.h>

// Declare histograms.
TH1F *h_pt_ratio = new TH1F("pt_ratio_right", "", 10, 0., 1.);
TCanvas *c = new TCanvas("c", "pt_Ratio", 800, 700);

void monotop_analysis::Begin(TTree * /*tree*/) {
  TString option = GetOption();
}

void monotop_analysis::SlaveBegin(TTree * /*tree*/) {
  TString option = GetOption();

  // PT ratio.
  h_pt_ratio->GetXaxis()->SetTitle("Pt(b)/(Pt(b) + Pt(mu))");
  h_pt_ratio->GetYaxis()->SetTitle("Nb events");
}

Bool_t monotop_analysis::Process(Long64_t entry) {
  // Get each event.
  GetEntry(entry);

  // Declare running indices.
  Int_t btag_i = -1;
  Int_t muon_i = -1;

  if (Muon_size > 0 && Jet_size > 0) {

    // Loop over jets and find the leading b-jet.
    for (Int_t i = 0; i < Jet_size; i++) {
      if (Jet_BTag[i] == 1 && btag_i < 0) {
        btag_i = i;
      }
    }

    // Loop over muons and find the highest pT muon.
    Double_t muon_pt_max = 0.0;
    for (Int_t i = 0; i < Muon_size; i++) {
      if (Muon_PT[i] > muon_pt_max) {
        muon_i = i;
        muon_pt_max = Muon_PT[i];
      }
    }

    Int_t sum_MissingET;
    for (Int_t i = 0; i < MissingET_size; i++) {
      sum_MissingET += MissingET_MET[i];
    }

    // Make cuts.
    bool btag_found = (btag_i > -1);
    if (btag_found) {
      Double_t delta_phi, btag_pT, btag_eta, muon_eta, muon_pT;

      delta_phi = abs(Muon_Phi[muon_i] - Jet_Phi[btag_i]);
      btag_pT = Jet_PT[btag_i];
      muon_pT = muon_pt_max;
      btag_eta = Jet_Eta[btag_i];
      muon_eta = Muon_Eta[muon_i];

      bool pt_cut = (muon_pT > 33.0 && btag_pT > 70.0);
      bool delta_phi_cut = (delta_phi < 1.7);
      bool btag_eta_cut = (btag_eta < 2.5);
      bool mu_eta_cut = (muon_eta < 2.1);
      bool et_miss_cut = (sum_MissingET > 100.0);
      bool passed_cuts = (delta_phi_cut && pt_cut && mu_eta_cut &&
                          btag_eta_cut && et_miss_cut);

      // Calculate R.
      Double_t pt_ratio = ((btag_pT) / (btag_pT + muon_pT));

      // Fill histograms if all cuts are passed.
      if (passed_cuts) {
        h_pt_ratio->Fill(pt_ratio);
      }
    }
  }

  return kTRUE;
}

void monotop_analysis::SlaveTerminate() {
  TFile *f = new TFile("pt_hist_right.root", "RECREATE");
  // Normalize.
  h_pt_ratio->Scale(1/h_pt_ratio->Integral());
  h_pt_ratio->Draw();
  h_pt_ratio->Write();
  f->Close();
}

void monotop_analysis::Terminate() {}
