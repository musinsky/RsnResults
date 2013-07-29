// Jan Musinsky, Martin Vala
// 2013-07-29

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
  gSystem->SetBuildDir("tmp", kTRUE);

  gSystem->Load("libRIO");
  gSystem->Load("libGpad");
  gSystem->Load("lib/libRsnResults.so");

  gStyle->SetHistLineColor(kBlack);
  gStyle->SetGridColor(kGray);
}
