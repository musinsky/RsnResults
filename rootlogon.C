// Jan Musinsky, Martin Vala
// 2013-11-07

#ifndef __CINT__
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>

void rootlogon()
#endif
{
  if (gROOT->GetClass("TRsnResults")) {
    Printf("RsnResults libraries was already loaded");
    return;
  }

  gSystem->AddIncludePath("-Iinclude");
  gSystem->SetBuildDir("build", kTRUE);

  gSystem->Load("libRIO");
  gSystem->Load("libGpad");
  gSystem->Load("lib/libRsnResults.so");

  gStyle->SetHistLineColor(kBlack);
  gStyle->SetGridColor(kGray);
}
