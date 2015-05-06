{
  TFile *_file0 = TFile::Open("RsnOutput.root");
  sh = new TRsnSparseHandler(Unlike);

  sh->SetAxisFragments(1, TRsnUtils::RangeFragments(Unlike->Projection(1), 0.5));

  sh->SetAxisRangeUser(3, -0.091, 0.1);
  sh->SetAxisElement("IM");
  sh->Print();

  gg = sh->MakeGroupFragments();
  sh->AddFragmentElement(Unlike);
  sh->AddFragmentElement(LikePP);
  sh->AddFragmentElement(MixingMM);
  gg->Print("17");

  //  .L macros/Highlight.C+
  //  DrawHighlightFragments(gg, "MixingMM_IM")
}
