// Jan Musinsky, Martin Vala
// 2014-11-18

//void rootlogon()
{
  if (TClass::GetClass("TRsnResults")) {
    Printf("RsnResults libraries was already loaded");
    return;
  }

  gSystem->AddIncludePath("-Iinclude");
  gSystem->SetBuildDir("build", kTRUE);

  gSystem->Load("libGpad");
  gSystem->Load("lib/libRsnResults.so");
  gSystem->Load("lib/libRsnUtils.so");

  gStyle->SetGridColor(kGray);
}
