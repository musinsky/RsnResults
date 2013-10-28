// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-28

#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>
#include <TH1.h>

#include "TRsnFragment.h"
#include "TRsnGroup.h"

ClassImp(TRsnFragment)

//______________________________________________________________________________
TRsnFragment::TRsnFragment()
: TNamed(),
  fMin(0.),
  fMax(0.),
  fGroup(0),
  fElements(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnFragment::TRsnFragment(Double_t min, Double_t max, TRsnGroup *group)
: TNamed(),
  fMin(min),
  fMax(max),
  fGroup(group),
  fElements(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnFragment::~TRsnFragment()
{
  SafeDelete(fElements); // objects are deleted (IsOwner)
}
//______________________________________________________________________________
Int_t TRsnFragment::Compare(const TObject *obj) const
{
  const TRsnFragment *other = dynamic_cast<const TRsnFragment *>(obj);
  if (!other) {
    Error("Compare", "input object is not a TRsnFragment object");
    return 0;
  }

  if (GetMean() < other->GetMean()) return -1;
  else if (GetMean() > other->GetMean()) return 1;
  else return 0;
}
//______________________________________________________________________________
void TRsnFragment::Print(Option_t *option) const
{
  if (fElements) fElements->Print(option);
}
//______________________________________________________________________________
void TRsnFragment::AddElement(TObject *obj, const char *label)
{
  if (!obj) return;
  if (!fGroup) {
    Error("AddElement", "fragment %s without parrent group", GetName());
    return;
  }

  //  if (fGroup->FindElementLabel(label) >= 0) {
  if (FindElement(label)) {
    Error("AddElement", "already exist element label %s in fragment %s", label, GetName());
    return;
  }

  if (!fElements) {
    fElements = new TObjArray();
    fElements->SetName("Elements");
    fElements->SetOwner(kTRUE);
  }
  else if (fElements->Contains(obj)) {
    Error("AddElement", "already exist element %s in fragment %s", obj->GetName(), GetName());
    return;
  }

  if (TRsnGroup::GetListOfAllElements()->Contains(obj)) {
    Error("AddElement", "already exist element %s in another fragment", obj->GetName());
    return;
  }

  fElements->Add(obj);                     // do not sort this array, must stay this order
  TObjString *olabel = new TObjString(label);
  olabel->SetUniqueID((UInt_t)fElements->GetLast());
  fGroup->GetElementLabels()->Add(olabel); // do not sort this array, must stay this order

  if (obj->InheritsFrom(TH1::Class())) ((TH1 *)obj)->SetDirectory(0);

  obj->SetBit(kMustCleanup); // obj destructor will remove the object from fgAllElements
  TRsnGroup::GetListOfAllElements()->Add(obj);
}
//______________________________________________________________________________
TObject *TRsnFragment::FindElement(Int_t idx, Bool_t direct) const
{
  if (!fElements) return 0;
  if (direct) return fElements->At(idx); // UncheckedAt(idx);

  if (!fGroup) return 0;
  return FindElement(fGroup->FindElementLabel(idx));
}
//______________________________________________________________________________
TObject *TRsnFragment::FindElement(const char *label) const
{
  if (!fGroup || !fElements) return 0;
  Int_t idx = fGroup->FindElementLabel(label);
  if (idx < 0) return 0;
  return fElements->At(idx); // UncheckedAt(idx);
}
