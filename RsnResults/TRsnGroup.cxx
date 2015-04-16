// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-17

#include <TROOT.h>
#include <TClass.h>
#include <THashList.h>

#include "TRsnGroup.h"
#include "TRsnFragment.h"

ClassImp(TRsnGroup)

//______________________________________________________________________________
TRsnGroup::TRsnGroup()
: TNamed(),
  fFragments(0),
  fElementTags(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(const char *name, const char *title)
: TNamed(name, title),
  fFragments(0),
  fElementTags(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::~TRsnGroup()
{
  // Destructor
  SafeDelete(fFragments);   // objects are deleted also
  SafeDelete(fElementTags); // objects are deleted also
}
//______________________________________________________________________________
void TRsnGroup::Print(Option_t *option) const
{
  if (!fFragments) return;

  // print all fragments as table
  TIter nextTag(fElementTags);
  TObject *tag;
  Int_t cw = TString(option).Atoi(); // cell width
  if (cw < 6) cw = 8;

  // header of table
  printf("%*s", 3*cw-4, ""); // 4 characters for [%02d]
  while ((tag = nextTag()))
    printf("[%02d]%-*.*s", tag->GetUniqueID(), 2*cw-4, 2*cw-5, tag->GetName());
  printf("\n\n");

  // rows of table
  for (Int_t i = 0; i < fFragments->GetEntriesFast(); i++) {
    TRsnFragment *frag = (TRsnFragment *)fFragments->At(i);
    if (frag)
      printf("[%02d]%-*.*s", i, 2*cw-4, 2*cw-5, frag->GetName());
    else {
      printf("[%02d]none\n", i);
      continue;
    }
    nextTag.Reset();
    while ((tag = nextTag())) {
      TObject *elem = frag->FindElement(tag->GetName());
      if (elem) printf("%*.*s/%-*.*s", cw, cw-1, elem->GetName(), cw-1, cw-1, elem->IsA()->GetName());
      else      printf("%*.*s/%-*.*s", cw, cw-1, "none", cw-1, cw-1, "none");
    }
    printf("\n");
  }
}
//______________________________________________________________________________
TRsnFragment *TRsnGroup::MakeFragment(Double_t min, Double_t max)
{
  if (min > max) {
    Warning("MakeFragment", "max=%g must be greater or equal than min=%g", max, min);
    return 0;
  }

  if (!fFragments) {
    fFragments = new TObjArray();
    fFragments->SetName("Fragments");
    fFragments->SetOwner(kTRUE);
    fFragments->SetBit(kMustCleanup); // recursive remove from fCleanups
    gROOT->GetListOfCleanups()->Add(fFragments);
  }

  TRsnFragment *fragment = new TRsnFragment(min, max, this);
  fragment->SetBit(kMustCleanup); // recursive remove from fFragments
  fFragments->Add(fragment);
  return fragment;
}
//______________________________________________________________________________
TRsnFragment *TRsnGroup::FindFragment(Double_t inside) const
{
  TIter next(fFragments);
  TRsnFragment *frag;
  while ((frag = (TRsnFragment *)next())) {
    // exclude min, max
    if ((inside < frag->GetMax()) && !(inside <= frag->GetMin())) return frag;
    //    // include min, max
    //    if ((inside <= frag->GetMax()) && !(inside < frag->GetMin())) return frag;
  }
  return 0;
}
//______________________________________________________________________________
Int_t TRsnGroup::AddElementTag(const char *tag)
{
  if (FindElementTag(tag) != -1) {
    Warning("AddElementTag", "duplicate tag '%s' in group '%s'", tag, GetName());
    return -1;
  }

  TObjString *obj = new TObjString(tag);
  if (obj->GetString().IsWhitespace()) {
    Warning("AddElementTag", "no whitespace tag");
    delete obj;
    return -1;
  }

  if (!fElementTags) {
    fElementTags = new THashList();
    fElementTags->SetName("ElementTags");
    fElementTags->SetOwner(kTRUE);
  }

  fElementTags->Add(obj);
  Int_t idx = fElementTags->GetLast(); // all element tags consecutively
  obj->SetUniqueID(idx);
  return idx;
}
//______________________________________________________________________________
Int_t TRsnGroup::FindElementTag(const char *tag) const
{
  // fast find a unique tag
  // return list->IndexOf(list->FindObject(tag)); // slowly (2x looping)

  if (!fElementTags) return -1;
  TObjString *obj = (TObjString *)fElementTags->FindObject(tag);
  if (obj) return obj->GetUniqueID();
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

  //  if ((idx < 0) || (idx > fElementTags->GetLast())) return "";
  //  return fElementTags->At(idx)->GetName();
}
