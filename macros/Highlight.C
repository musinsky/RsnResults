// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-20

#include <TList.h>
#include <TGraph.h> // patched ROOT with Highlight for TGraph
#include <TAxis.h>
#include <TExec.h>
#include <TCanvas.h>

#include "TRsnGroup.h"
#include "TRsnFragment.h"

const TRsnGroup *group = 0;
TGraph *gr = 0;
const char *tagname = "";

//______________________________________________________________________________
void DrawHighlightFragments(const TRsnGroup *rsng, const char *whichtag = "")
{
  if (!rsng || !rsng->GetListOfFragments()) return;
  tagname = whichtag;

  group = rsng;
  TObjArray *fragments = group->GetListOfFragments();
  gr = new TGraph(fragments->GetEntriesFast());
  for (Int_t i = 0; i < fragments->GetEntriesFast(); i++) {
    TRsnFragment *frag = group->FragmentAt(i);
    if (frag) gr->SetPoint(i, frag->GetMean(), 0.91);
  }
  //  TString cmd = TString::Format("(TGraph *)0x%lx", (ULong_t)g);
  //  Printf("cmd = %s", cmd.Data());
  //  cmd = TString::Format("((TRsnGroup *)0x%lx)->HighlightFragment(%s)",
  //                        (ULong_t)this, cmd.Data());
  //  Printf("cmd = %s", cmd.Data());
  //  TExec *ex = new TExec("ex", cmd.Data());

  TExec *ex = new TExec("ex", "HighlightFragment()");
  gr->GetListOfFunctions()->Add(ex);
  gr->SetHighlight(kTRUE);

  gr->SetTitle();
  gr->SetMarkerStyle(20);
  gr->GetYaxis()->SetTickLength(0.0);
  gr->SetBit(kCanDelete);
  gr->Draw("AP");
  gr->GetXaxis()->SetTitle("bla_bla");
}
//______________________________________________________________________________
void HighlightFragment()
{
  // TODO templates: what and how highlighing
  if (!gPad || !gr) return;

  // not correct
  TVirtualPad *ph = (TVirtualPad *)gPad->FindObject("ph");
  if (!ph) {
    ph = new TPad("ph", "ph", 0.0, 0.2, 1.0, 1.0);
    ph->SetFillColor(kBlue-10);
    ph->Draw();
  }

  Int_t ih = gr->GetHighlightPoint();
  if (ih == -1) return;
  TRsnFragment *frag = group->FragmentAt(ih);
  if (!frag) return;

  TVirtualPad *save = gPad;
  ph->cd();
  TObject *element = frag->FindElement(tagname);
  if (!element) ph->Clear();
  else element->Draw();
  save->cd();
}
