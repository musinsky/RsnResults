// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-04-20

#include <TObjArray.h>
#include <TROOT.h>
#include <TClass.h>
#include <TH1.h>

#include "TRsnFragment.h"
#include "TRsnGroup.h"

TList *TRsnFragment::fgAllElements = 0;

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
  // ToDo: improve
  if (!fGroup || !fElements) return;

  TObject *obj;
  for (Int_t i = 0; i < fElements->GetEntriesFast(); i++) {
    obj = fElements->At(i);
    if (obj) Printf("%02d  %s \t %s \t %s \t %s at: %p", i, fGroup->FindElementTag(i),
                    obj->IsA()->GetName(), obj->GetName(), obj->GetTitle(), (void *)obj);
    else     Printf("%02d  %s \t %s", i, fGroup->FindElementTag(i), "empty slot");
  }
}
//______________________________________________________________________________
TList *TRsnFragment::GetListOfAllElements()
{
  // static function
  return fgAllElements;
}
//______________________________________________________________________________
void TRsnFragment::AddElement(TObject *obj, const char *tag)
{
  if (!obj) {
    Error("AddElement", "object is null");
    return;
  }
  if (!obj->IsOnHeap()) {
    SysError("AddElement", "object is not on the heap");
    return;
  }
  if (!fGroup) {
    Error("AddElement", "fragment without a parent group");
    return;
  }

  if (FindElement(tag)) {
    Warning("AddElement", "already exist element with tag: %s in fragment", tag);
    return;
  }

  if (!fElements) {
    fElements = new TObjArray();
    fElements->SetName("Elements");
    fElements->SetOwner(kTRUE);
  }
  else if (fElements->Contains(obj)) {
    Warning("AddElement", "already exist element: %s in fragment", obj->GetName());
    return;
  }

  if (!fgAllElements) {
    fgAllElements = new TList();
    fgAllElements->SetName("AllElements");
    gROOT->GetListOfCleanups()->Add(fgAllElements);
  }
  else if (fgAllElements->Contains(obj)) {
    Warning("AddElement", "already exist element: %s in another fragment", obj->GetName());
    return;
  }

  // ToDo: LockTags, new tag only from group
  Int_t idx = fGroup->FindElementTag(tag);
  if (idx < 0) idx = fGroup->AddElementTag(tag);
  fElements->AddAt(obj, idx);

  obj->SetBit(kMustCleanup); // recursive remove from fgAllElements
  fgAllElements->Add(obj);

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
  if (!obj) return ""; // must be, otherwise return tag of the first empty slot of fElements
  if (!fGroup || !fElements) return "";
  Int_t idx = fElements->IndexOf(obj);
  if (idx < 0) return "";
  return fGroup->FindElementTag(idx);
}
