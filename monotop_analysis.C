#define monotop_analysis_cxx

#include "monotop_analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TParticle.h>

void monotop_analysis::Begin(TTree * /*tree*/) {
  TString option = GetOption();
}

void monotop_analysis::SlaveBegin(TTree * /*tree*/) {
  TString option = GetOption();

  // Make Canvases.
  TCanvas *c1 = new TCanvas("c1", "plots", 1200, 700);
  c1->Divide(2,2);

  // Define histograms.
  // m_jjb
  h_mass = new TH1F("M", "dijet + b Mass", 100, 90., 700.);
  h_mass->GetXaxis()->SetTitle("M_jjb (GeV/c^2)");
  h_mass->GetYaxis()->SetTitle("Nb events");

  // jet_energy
  h_energy = new TH1F("E", "dijet + b Energy", 100, 100., 1500.);
  h_energy->GetXaxis()->SetTitle("E_jjb (GeV)");
  h_energy->GetYaxis()->SetTitle("Nb events");

  // jet_pt
  h_pt = new TH1F("PT", "dijet + b Pt", 100, 25., 550.);
  h_pt->GetXaxis()->SetTitle("PT_jjb (GeV/c)");
  h_pt->GetYaxis()->SetTitle("Nb events");
}

Bool_t monotop_analysis::Process(Long64_t entry) {
  GetEntry(entry);
  // Declare running indices.
  Int_t	btag_i = -1;
  Int_t	jet1_i = -1;
  Int_t	jet2_i = -1;

  TLorentzVector btag;
  TLorentzVector jet1;
  TLorentzVector jet2;

  if (Jet_size > 2) {
    // ID the leading btagged jet and the 2 leading non-btagged jets.
    for (Int_t i = 0; i < Jet_size; i++) {
      if (Jet_BTag[i] == 1 && btag_i < 0) {
        btag_i = i;
        // printf("btag found as index %d \n", i);
        continue;
      }

      if (Jet_BTag[i] == 0 && jet1_i < 0) {
        jet1_i = i;
        //printf("jet1 found as index %d \n", i);
        continue;
      }

      if (Jet_BTag[i] == 0 && jet1_i >= 0 && jet2_i < 0) {
        jet2_i = i;
        //printf("jet2 found as index %d \n", i);
      }
    }

    // Construct TLorentzVectors for the tagged b-jet and the two leading non-b-jets.
    if (btag_i > -1 && jet1_i > -1 && jet2_i > -1) {
      btag.SetPtEtaPhiM(Jet_PT[btag_i], Jet_Eta[btag_i], Jet_Phi[btag_i], Jet_Mass[btag_i]);
      jet1.SetPtEtaPhiM(Jet_PT[jet1_i], Jet_Eta[jet1_i], Jet_Phi[jet1_i], Jet_Mass[jet1_i]);
      jet2.SetPtEtaPhiM(Jet_PT[jet2_i], Jet_Eta[jet2_i], Jet_Phi[jet2_i], Jet_Mass[jet2_i]);
    }

    TLorentzVector multijet = btag + jet1 + jet2;

    // Calculate mass_j_j_b.
    Double_t mass_j_j_b = multijet.M();

    // Calculate jet_energy.
    Double_t jet_energy = multijet.E();

    // Calculate jet_pt.
    Double_t jet_pt = multijet.Pt();

    // Fill histograms
    h_mass->Fill(mass_j_j_b);
    h_energy->Fill(jet_energy);
    h_pt->Fill(jet_pt);
  }
  return kTRUE;
}

void monotop_analysis::SlaveTerminate() {
  c1->cd(1);
  h_mass->Draw();
  c1->cd(2);
  h_energy->Draw();
  c1->cd(3);
  h_pt->Draw();
  c1->Print("monotop_delphes_plots.jpeg");
}

void monotop_analysis::Terminate() {}
