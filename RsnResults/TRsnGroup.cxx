// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-10-10

#include <TROOT.h>
#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>

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
void TRsnGroup::Print(Option_t * /*option*/) const
{
  // ToDo
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
TRsnFragment *TRsnGroup::MakeFragment(Double_t min, Double_t max)
{
  if (min > max) {
    Warning("MakeFragment", "max=%f must be greater or equal than min=%f", max, min);
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
  Int_t idx = fElementTags->GetLast();
  obj->SetUniqueID((UInt_t)idx);
  return idx;
}
//______________________________________________________________________________
Int_t TRsnGroup::FindElementTag(const char *tag) const
{
  // fast find a unique tag
  // return list->IndexOf(list->FindObject(tag)); // slowly (2x looping)

  if (!fElementTags) return -1;
  TObjString *obj = (TObjString *)fElementTags->FindObject(tag);
  if (obj) return (Int_t)obj->GetUniqueID();
  return -1;
}
//______________________________________________________________________________
const char *TRsnGroup::FindElementTag(Int_t idx) const
{
  if (!fElementTags) return "";
  if ((idx < 0) || (idx > fElementTags->GetLast())) return "";

  return fElementTags->At(idx)->GetName();

  //  TIter next(fElementTags);
  //  TObjString *obj;
  //  while ((obj = (TObjString *)next()))
  //    if (((Int_t)obj->GetUniqueID()) == idx) return obj->GetName();
  //  return "";
}
