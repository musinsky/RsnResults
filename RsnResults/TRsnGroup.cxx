// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-11-06

#include <TCanvas.h>
#include <TROOT.h>
#include <TGClient.h>
#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>

#include "TRsnGroup.h"
#include "TRsnFragment.h"

ClassImp(TRsnGroup)

//______________________________________________________________________________
TRsnGroup::TRsnGroup()
: TGraphRsnErrors(),
  fFragments(0),
  fElementTags(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n)
: TGraphRsnErrors(n),
  fFragments(0),
  fElementTags(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Float_t *x, const Float_t *y,
                     const Float_t *ex, const Float_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0),
  fElementTags(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Double_t *x, const Double_t *y,
                     const Double_t *ex, const Double_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0),
  fElementTags(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(const char *filename, const char *format, Option_t *option)
: TGraphRsnErrors(filename, format, option),
  fFragments(0),
  fElementTags(0)
{
  // Constructor reading input from filename
}
//______________________________________________________________________________
TRsnGroup::~TRsnGroup()
{
  // Destructor
  SafeDelete(fFragments);   // objects are deleted also
  SafeDelete(fElementTags); // objects are deleted also
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
void TRsnGroup::Print(Option_t * /*option*/) const
{
  // ToDo: improve
  if (!fElementTags) return;

  TObjString *obj;
  for (Int_t i = 0; i < fElementTags->GetSize(); i++) {
    obj = (TObjString *)fElementTags->At(i);
    if (obj) Printf("%02d  %02d  %s \t %02d  %s", i, (Int_t)obj->GetUniqueID(), obj->GetName(),
                    FindElementTag(FindElementTag(i)), FindElementTag(i));
    else     Printf("%02d  %s \t %s", i, FindElementTag(i), "empty slot (impossible)");
  }
}
//______________________________________________________________________________
void TRsnGroup::AddAtFragmentBla(TObject *obj, Int_t idx)
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
    Error("MakeFragment", "max %f must be greater or equal than min %f", max, min);
    return 0;
  }

  if (!fFragments) {
    fFragments = new TObjArray();
    fFragments->SetName("Fragments");
    fFragments->SetOwner(kTRUE);
    gROOT->GetListOfCleanups()->Add(fFragments);
  }

  TRsnFragment *fragment = new TRsnFragment(min, max, this);
  fragment->SetBit(kMustCleanup); // recursive remove from fFragments
  fFragments->Add(fragment);
  return fragment;
}
//______________________________________________________________________________
Int_t TRsnGroup::AddElementTag(const char *tag)
{
  if (FindElementTag(tag) >= 0) {
    Warning("AddElementTag", "already exist tag with name: %s in group", tag);
    return -1; // out of bounds
  }

  if (!fElementTags) {
    fElementTags = new THashList();
    fElementTags->SetName("ElementTags");
    fElementTags->SetOwner(kTRUE);
  }

  TObjString *obj = new TObjString(tag);
  fElementTags->Add(obj);
  Int_t idx = fElementTags->LastIndex(); // GetLast != GetSize if tag name are not unique
  obj->SetUniqueID((UInt_t)idx);
  return idx;
}
//______________________________________________________________________________
Int_t TRsnGroup::FindElementTag(const char *tag) const
{
  // fast find a tag
  // return list->IndexOf(list->FindObject(tag)); // slowly (2x looping)

  if (!fElementTags) return -1; // out of bounds
  TObjString *obj = (TObjString *)fElementTags->FindObject(tag);
  if (obj) return (Int_t)obj->GetUniqueID();
  return -1;
}
//______________________________________________________________________________
const char *TRsnGroup::FindElementTag(Int_t idx) const
{
  if (!fElementTags) return "";
  TIter next(fElementTags);
  TObjString *obj;
  while ((obj = (TObjString *)next()))
    if (((Int_t)obj->GetUniqueID()) == idx) return obj->GetName();
  return "";

  // same as
  // return fElementTags->At(idx)->GetName();
}
