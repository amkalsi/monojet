// An analyzer to produce the b-tagged jet / b-tagged + mu PT ratio.
// USAGE:
// gSystem->Load("<path_to_delphes>/Delphes/libDelphes.so");
// .x PTRatioAnalyzer.C("input_file.root", "saved_histogram_name.root", "plot_name", nbins);

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#else
class ExRootTreeReader;
class ExRootResult;
#endif

void PTRatioAnalyzer(const char *file_name, const char *save_name, const char *plot_name, int nbins) {
  gSystem->Load("/home/thompson/MG5_aMC_2_6_0/Delphes/libDelphes.so");

  // Create a chain of root trees.
  TChain chain("Delphes");
  chain.Add(file_name);

  // Create object of class ExRootTreeReader.
  ExRootTreeReader *tree_reader = new ExRootTreeReader(&chain);
  Long64_t number_of_entries = tree_reader->GetEntries();

  // Get pointers to branches used in this analysis.
  TClonesArray *branch_mu = tree_reader->UseBranch("Muon");
  TClonesArray *branch_jet = tree_reader->UseBranch("Jet");
  TClonesArray *branch_met = tree_reader->UseBranch("MissingET");

  // Book histograms.
  TH1F *h_pt_ratio = new TH1F(plot_name, "", nbins, 0., 1.);
  h_pt_ratio->GetXaxis()->SetTitle("Pt(b)/(Pt(b) + Pt(mu))");
  h_pt_ratio->GetYaxis()->SetTitle("Nb events");

  printf("next event with size %f \n", number_of_entries);

  // Event loop.
  for(Int_t entry = 0; entry < number_of_entries; ++entry) {

    tree_reader->ReadEntry(entry);

    // Declare physics objects.
    Muon *muon;
    Jet *jet;
    MissingET *met;

    // Declare running indices.
    bool btag_found = false;
    Double_t muon_pt = 0.0;
    Double_t muon_phi, muon_eta;
    Double_t btag_pt, btag_phi, btag_eta;
    Int_t muon_n = branch_mu->GetEntries();
    Int_t jet_n = branch_jet->GetEntries();
    Int_t met_n = branch_met->GetEntries();

    if (muon_n > 0 &&  jet_n > 0) {

      // Loop over jets and find the leading b-jet.
      for (Int_t i = 0; i < jet_n; i++) {
        jet = (Jet*) branch_jet->At(i);
        if (jet->BTag == 1 && !btag_found) {
          btag_found = true;
          btag_pt = jet->PT;
          btag_phi = jet->Phi;
          btag_eta = jet->Eta;
        }
      }

      // Loop over muons and find the highest pT muon.
      for (Int_t i = 0; i < muon_n; i++) {
        muon = (Muon*) branch_mu->At(i);
        if (muon->PT > muon_pt) {  // Get the maximum PT muon.
          muon_pt = muon->PT;
          printf("new muon pt = %f \n", muon_pt);
          muon_phi = muon->Phi;
          muon_eta = muon->Eta;
        }
      }

      Int_t sum_met;
      for (Int_t i = 0; i < met_n; i++) {
        met = (MissingET*) branch_met->At(i);
        sum_met += met->MET;
      }
      printf("MET = %f \n", sum_met);

      // Make cuts.
      if (btag_found) {
        Double_t delta_phi = abs(muon_phi - btag_phi);

        bool pt_cut = (muon_pt > 33.0 && btag_pt > 70.0);
        bool delta_phi_cut = (abs(delta_phi) < 1.7);
        bool btag_eta_cut = (abs(btag_eta) < 2.5);
        bool mu_eta_cut = (abs(muon_eta) < 2.1);
        bool missing_et_cut = (sum_met > 100.0);

        bool passed_cuts = (delta_phi_cut && pt_cut && mu_eta_cut &&
                            btag_eta_cut && missing_et_cut);

        // Fill histograms if all cuts are passed.
        if (passed_cuts) {
          printf("passed cuts, pt ratio is %f \n", pt_ratio);

          // Calculate R.
          printf("Calculating R with btag_pt = %f and muon_pt = %f \n", btag_pt, muon_pt);
          Double_t pt_ratio = ((btag_pt) / (btag_pt + muon_pt));
          h_pt_ratio->Fill(pt_ratio);
        }
      }  // End if (btag_found)
    }  // End if (muon_n > 0 && jet_n > 0)

  }

  // Draw hist and save.
  TCanvas *c = new TCanvas();

  h_pt_ratio->Scale(1/h_pt_ratio->Integral());
  h_pt_ratio->Draw();

  TFile *f = new TFile(save_name,"RECREATE");
  h_pt_ratio->Write("",TObject::kOverwrite);
}
