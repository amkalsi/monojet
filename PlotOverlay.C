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

void PlotOverlay(char *file1, char *file2) {
  gStyle->SetOptStat(0);
  TCanvas *c = new TCanvas("c", "PT ratio", 800, 700);
  TH1F *h1 = new TH1F("", "", 100, 0., 1.);
  TH1F *h2 = new TH1F("", "", 100, 0., 1.);

  TFile *f1 = new TFile(file1);
  TIter next(f1->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1F")) continue;
    h1 = (TH1F*)key->ReadObj();
  }

  TFile *f2 = TFile::Open(file2);
  TIter next2(f2->GetListOfKeys());
  TKey *key2;
  while ((key2 = (TKey*)next2())) {
    TClass *cl2 = gROOT->GetClass(key2->GetClassName());
    if (!cl2->InheritsFrom("TH1F")) continue;
    h2 = (TH1F*)key2->ReadObj();
  }

  h1->SetTitle("pT Ratio, inclusive neutrino (leptonic W - GEN level)");
  h1->GetXaxis()->SetTitle("Pt(b)/(Pt(b) + Pt(mu) + Pt(vm))");
  h1->GetYaxis()->SetTitle("Nb events");
  h1->GetYaxis()->SetRangeUser(0., 250.);
  h1->SetLineColor(kRed);
  h1->SetLineWidth(4);

  h2->SetLineColor(kBlue);
  h2->SetLineWidth(4);

  h1->Draw();
  h2->Draw("SAME");

  TLegend *legend = new TLegend(0.1, 0.65, 0.48, 0.82);
  legend->AddEntry(h1, "Left Handed", "f");
  legend->AddEntry(h2, "Right Handed", "f");
  legend->Draw();
}
