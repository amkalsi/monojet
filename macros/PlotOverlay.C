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

void PlotOverlay(const char *file1, const char *file2, const char *title) {
  //gStyle->SetOptStat(0);

  TCanvas *c = new TCanvas("c", "PT ratio", 800, 700);
  TH1F *h1 = new TH1F("", "", 10, 0., 1.);
  TH1F *h2 = new TH1F("", "", 10, 0., 1.);

  TFile *f1 = new TFile(file1);
  TIter next(f1->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    cout << "Found object: " << key->GetClassName() << endl;
    if (!cl->InheritsFrom("TH1F")) continue;
    h1 = (TH1F*)key->ReadObj();
  }

  TFile *f2 = TFile::Open(file2);
  TIter next2(f2->GetListOfKeys());
  TKey *key2;
  while ((key2 = (TKey*)next2())) {
    TClass *cl2 = gROOT->GetClass(key2->GetClassName());
    cout << "Found object: " << key2->GetClassName() << endl;
    if (!cl2->InheritsFrom("TH1F")) continue;
    h2 = (TH1F*)key2->ReadObj();
  }

  h1->SetTitle(title);
  h1->GetXaxis()->SetTitle("Pt(b)/(Pt(b) + Pt(mu)");
  h1->GetYaxis()->SetTitle("");
  h1->GetYaxis()->SetRangeUser(0., .4);

  h1->SetLineColorAlpha(9, 1);
  h1->SetLineWidth(4);
  h1->SetFillColorAlpha(9, 0.1);

  h2->SetLineColorAlpha(46, 1);
  h2->SetFillColorAlpha(46, 0.1);
  h2->SetLineWidth(4);

  h1->Draw();
  h2->Draw("SAME");

  TLegend *legend = new TLegend(0.1, 0.65, 0.3, 0.75);
  legend->AddEntry(h1, "LH Top", "f");
  legend->AddEntry(h2, "RH Top", "f");
  legend->Draw();
}
