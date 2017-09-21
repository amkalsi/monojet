#define monotop_analysis_cxx

#include "monotop_analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TParticle.h>

// Declare histograms.
TH1F *h_pt_ratio;

void monotop_analysis::Begin(TTree * /*tree*/) {
  TString option = GetOption();
}

void monotop_analysis::SlaveBegin(TTree * /*tree*/) {
  TString option = GetOption();

  // PT ratio.
  h_pt_ratio = new TH1F("pt_ratio_right", "pT ratio (Leptonic W, Right-Handed)", 30, 0., 1.);
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

    // Sum the MET in the event for the MET cut.

    // Make cuts.
    bool btag_found = (btag_i > -1);
    if (btag_found) {
      Float_t delta_phi, btag_pT, btag_eta, muon_pT;

      delta_phi = abs(Muon_Phi[muon_i] - Jet_Phi[btag_i]);
      btag_pT = Jet_PT[btag_i];
      muon_pT = muon_pt_max;
      btag_eta = Jet_Eta[btag_i];

      bool pt_cut = (muon_pT > 33.0 && btag_pT > 70.0);
      bool delta_phi_cut = (delta_phi < 1.7);
      bool btag_eta_cut = (btag_eta < 2.5);
      bool passed_cuts = (btag_cut && muon_cut && delta_phi_cut && pt_cut && btag_eta_cut);

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
  TFile *f_right = new TFile("pt_hist_right.root", "RECREATE");
  h_pt_ratio->Write();
  f_right->Close();
}

void monotop_analysis::Terminate() {}
