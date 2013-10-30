// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-30

#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>
#include <TString.h>
#include <TClass.h>
#include <TH1.h>

#include "TRsnFragment.h"
#include "TRsnGroup.h"

ClassImp(TRsnFragment)

//______________________________________________________________________________
TRsnFragment::TRsnFragment()
: TObject(),
  fMin(0.),
  fMax(0.),
  fGroup(0),
  fElements(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnFragment::TRsnFragment(Double_t min, Double_t max, TRsnGroup *group)
: TObject(),
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
  // Destructor
  SafeDelete(fElements); // objects are deleted also
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
void TRsnFragment::Print(Option_t * /*option*/) const
{
  if (!fElements) return;

  TObject *obj;
  for (Int_t i = 0; i < fElements->GetEntriesFast(); i++) {
    obj = fElements->At(i);
    if (obj) Printf("%02d  %s \t %s \t %s \t %s at: %p", i, fGroup->FindElementTag(i),
                    obj->IsA()->GetName(), obj->GetName(), obj->GetTitle(), obj);
    else     Printf("%02d  %s \t %s", i, fGroup->FindElementTag(i), "empty slot");
  }
}
//______________________________________________________________________________
void TRsnFragment::AddElement(TObject *obj, const char *tag)
{
  if (!obj) return;
  if (!fGroup) {
    Error("AddElement", "fragment without parent group");
    return;
  }

  if (FindElement(tag)) {
    Error("AddElement", "already exist element with tag: %s in fragment", tag);
    return;
  }

  if (!fElements) {
    fElements = new TObjArray();
    fElements->SetName("Elements");
    fElements->SetOwner(kTRUE);
  }
  else if (fElements->Contains(obj)) {
    Error("AddElement", "already exist element: %s in fragment", obj->GetName());
    return;
  }

  if (TRsnGroup::GetListOfAllElements()->Contains(obj)) {
    Error("AddElement", "already exist element: %s in another fragment", obj->GetName());
    return;
  }

  Int_t idx = fGroup->FindElementTag(tag);
  if (idx < 0) {
    fElements->Add(obj);                  // do not sort this array, must stay this order
    // ToDo
    // poznamka, ze tu addelementtag a ne v group
    // komentare pomazat
    // Error Warning Info ?!
    TObjString *idStr = new TObjString(tag); // creating new element tag
    idStr->SetUniqueID((UInt_t)fElements->GetLast());
    fGroup->GetElementTags()->Add(idStr); // do not sort this array, must stay this order
  }
  else fElements->AddAt(obj, idx);

  obj->SetBit(kMustCleanup); // obj destructor will remove the object from fgAllElements
  TRsnGroup::GetListOfAllElements()->Add(obj);

  if (obj->InheritsFrom(TH1::Class())) ((TH1 *)obj)->SetDirectory(0);
}
//______________________________________________________________________________
TObject *TRsnFragment::FindElement(const char *tag) const
{
  // fast find an object using its tag

  if (!fGroup || !fElements) return 0;
  Int_t idx = fGroup->FindElementTag(tag);
  if (idx < 0) return 0;
  return fElements->UncheckedAt(idx);
}
//______________________________________________________________________________
const char *TRsnFragment::FindTag(const TObject *obj) const
{
  if (!obj) return ""; // must be, otherwise return tag of first empty slot
  if (!fGroup || !fElements) return "";

  Int_t idx = fElements->IndexOf(obj);
  if (idx < 0) return "";
  return fGroup->FindElementTag(idx);
}
