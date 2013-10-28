// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-28

#include <TCanvas.h>
#include <TROOT.h>
#include <TGClient.h>
#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>

#include "TRsnGroup.h"
#include "TRsnFragment.h"

TList *TRsnGroup::fgAllElements = 0;

ClassImp(TRsnGroup)

//______________________________________________________________________________
TRsnGroup::TRsnGroup()
: TGraphRsnErrors(),
  fFragments(0),
  fElementLabels(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n)
: TGraphRsnErrors(n),
  fFragments(0),
  fElementLabels(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Float_t *x, const Float_t *y,
                     const Float_t *ex, const Float_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0),
  fElementLabels(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Double_t *x, const Double_t *y,
                     const Double_t *ex, const Double_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0),
  fElementLabels(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(const char *filename, const char *format, Option_t *option)
: TGraphRsnErrors(filename, format, option),
  fFragments(0),
  fElementLabels(0)
{
  // Constructor reading input from filename
}
//______________________________________________________________________________
TRsnGroup::~TRsnGroup()
{
  // Destructor
  SafeDelete(fFragments);     // objects are deleted (IsOwner)
  SafeDelete(fElementLabels); // objects are deleted (IsOwner)
}
//______________________________________________________________________________
void TRsnGroup::Flash(Option_t *option)
{
  TString opt = option;
  opt.ToLower();
  if      (opt.Contains("exam1")) Printf("%s", option);
  else if (opt.Contains("exam2")) Printf("%s", option);
  TVirtualPad *save = gPad;

  TCanvas *c = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("c_flash");
  if (!GetShowFlash() && c) { // close canvas after disable SetShowFlash
    delete c;
    if (save) save->cd();
    return;
  }
  if (!c) {
    Int_t ww = 600;
    c = new TCanvas("c_flash", "", gClient->GetDisplayWidth() - ww, 0, ww, ww*1.20);
    c->Divide(1, 2);
  }
  else c->Clear("D"); // subpads are cleared but not deleted

  TObject *obj;
  c->cd(1);
  obj = fFragments->At(fFlashPoint);
  if (obj) obj->Draw();
  c->cd(2);
  obj = fFragments->At(fNpoints-1-fFlashPoint);
  if (obj) obj->Draw();
  c->Update();

  if (save) save->cd();
}
//______________________________________________________________________________
TList *TRsnGroup::GetListOfAllElements()
{
  // static function
  return fgAllElements;
}
//______________________________________________________________________________
void TRsnGroup::AddAtFragment(TObject* obj, Int_t idx)
{
  if (fNpoints <= 0) return;
  if (!fFragments) {
    fFragments = new TObjArray(fNpoints);
    fFragments->SetName("Fragments");
    fFragments->SetOwner(kTRUE);
  }
  fFragments->AddAt(obj, idx);
}
//______________________________________________________________________________
TRsnFragment *TRsnGroup::MakeFragment(Double_t min, Double_t max)
{
  if (min > max) {
    Error("MakeFragment", "max %f must be greater than min %f", max, min);
    return 0;
  }

  if (!fgAllElements) {
    fgAllElements = new TList();
    fgAllElements->SetName("AllElements");
    gROOT->GetListOfCleanups()->Add(fgAllElements);
  }

  if (!fFragments) {
    fFragments = new TObjArray();
    fFragments->SetName("Fragments");
    fFragments->SetOwner(kTRUE);
  }
  if (!fElementLabels) {
    fElementLabels = new THashList();
    fElementLabels->SetName("ElementLabels");
    fElementLabels->SetOwner(kTRUE);
  }

  TRsnFragment *fragment = new TRsnFragment(min, max, this);
  fFragments->Add(fragment);
  return fragment;
}
//______________________________________________________________________________
const char *TRsnGroup::FindElementLabel(Int_t element) const
{
  if (!fElementLabels) return "";
  TObjString *obj;
  TIter next(fElementLabels);
  while ((obj = (TObjString *)next()))
    if (((Int_t)obj->GetUniqueID()) == element) return obj->GetName();

  return "";
}
//______________________________________________________________________________
Int_t TRsnGroup::FindElementLabel(const char *label) const
{
  if (!fElementLabels) return -1; // out of bounds
  TObjString *obj = (TObjString *)fElementLabels->FindObject(label);
  if (obj) return (Int_t)obj->GetUniqueID();

  return -1;
}
