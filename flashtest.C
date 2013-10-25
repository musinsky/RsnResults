// Jan Musinsky, Martin Vala
// 2013-10-21

#ifndef __CINT__
#include <TH1.h>
#include <TRandom.h>

void test()
#endif
{
  Int_t n = 20;
  Double_t x[n], y[n];
  TH1D *h;

  // first generate group/graph
  for (Int_t i = 0; i < n; i++) {
    x[i] = i;
    y[i] = gRandom->Uniform(0, n);
  }
  TRsnGroup *group = new TRsnGroup(n, x, y);

  // second generate some histos
  for (Int_t i = 0; i < n; i++) {
    h = new TH1D(Form("h_%02d", i), "", 100, 0, 100);
    for (Int_t j = 0; j < 1000; j++)
      h->Fill(gRandom->Gaus(100./n*i, 10.0));
    group->AddAtFragment(h, i);
  }

  group->SetMarkerStyle(20);
  group->Draw("ALP");
  group->SetShowFlash();
  TGraphRsnErrors::AroundFlash(kFALSE);
}
