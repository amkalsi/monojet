// PlotOverlay.C
// A macro that takes 2 root files that each store a TH1F and overlays the two plots.

// file1
// file2
// Optional: plot title
// Optional: x-axis title
// Optional: y-axis title
// Optional: x-axis range
// Optional: y-axis range
// Optional: file1 legend title
// Optional: file2 legend title

void PlotOverlay(const char *file1, const char *file2) {
  TCanvas *c = new TCanvas("c", "PT ratio", 800, 700);

  gStyle->SetOptStat(0);
  TFile f1(file1);

  // Get the name of the histogram in file1 and make a local auto equal to it.

  pt_r_vm_right->SetTitle("pT Ratio, inclusive neutrino (leptonic W - GEN level)");
  pt_r_vm_right->GetXaxis()->SetTitle("Pt(b)/(Pt(b) + Pt(mu) + Pt(vm))");
  pt_r_vm_right->GetYaxis()->SetTitle("Nb events");
  pt_r_vm_right->GetYaxis()->SetRangeUser(0., 250.);

  pt_r_vm_right->SetLineColor(kRed);
  pt_r_vm_right->SetLineWidth(4);

  pt_r_vm_right->Draw("HIST");

  TFile f2(file2);

  // Get the name of the histogram in file2 and make a local auto equal to it.

  pt_r_vm_left->SetLineColor(kBlue);
  pt_r_vm_left->SetLineWidth(4);

  pt_r_vm_left->Draw("HIST same");

  TLegend *legend = new TLegend(0.1, 0.65, 0.48, 0.82);
  legend->AddEntry(pt_r_vm_left, "Left Handed", "f");
  legend->AddEntry(pt_r_vm_right, "Right Handed", "f");
  legend->Draw();

  f1->Close();
  f2->Close();
}
