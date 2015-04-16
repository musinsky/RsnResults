// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-17

#include <TROOT.h>

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
void TRsnFragment::Print(Option_t *option) const
{
  if (!fGroup || !fElements) return;

  Printf("%s\tmin=%f\tmax=%f\tparent group='%s'\tnumber of elements=%d",
         GetName(), fMin, fMax, fGroup->GetName(), fElements->GetEntries());

  TString opt(option);
  opt.ToLower();
  if (!opt.Contains("all")) return;

  for (Int_t i = 0; i < fElements->GetEntriesFast(); i++) {
    TObject *obj = fElements->At(i);
    printf("[%02d]%s\t", i, fGroup->FindElementTag(i));
    if (obj) obj->ls();
    else printf("none\n");
  }
}
//______________________________________________________________________________
TList *TRsnFragment::GetListOfAllElements()
{
  // static function
  return fgAllElements;
}
//______________________________________________________________________________
Int_t TRsnFragment::AddElement(TObject *obj, const char *tag)
{
  if (!obj) {
    Warning("AddElement", "no object");
    return -1;
  }
  if (TString(tag).IsWhitespace()) {
    Warning("AddElement", "no whitespace tag");
    return -1;
  }
  if (!fGroup) {
    Warning("AddElement", "fragment without a parent group");
    return -1;
  }
  if (FindElement(tag)) {
    Warning("AddElement", "duplicate tag '%s' with other element '%s' in this fragment",
            tag, FindElement(tag)->GetName());
    return -1;
  }
  if (!TString(FindElement(obj)).IsWhitespace()) {
    Warning("AddElement", "duplicate element '%s' with other tag '%s' in this fragment",
            obj->GetName(), FindElement(obj));
    return -1;
  }

  if (!fgAllElements) {
    fgAllElements = new TList();
    fgAllElements->SetName("AllElements");
    fgAllElements->SetBit(kMustCleanup); // recursive remove from fCleanups
    gROOT->GetListOfCleanups()->Add(fgAllElements);
  } else if (fgAllElements->Contains(obj)) {
    Warning("AddElement", "already exists element '%s' in another fragment", obj->GetName());
    return -1;
  }

  // tag is always unique
  // ToDo: LockTags, new tag only from group
  Int_t idx = fGroup->FindElementTag(tag);       // exist
  if (idx < 0) idx = fGroup->AddElementTag(tag); // new

  if (!fElements) {
    fElements = new TObjArray();
    fElements->SetName("Elements");
    fElements->SetOwner(kTRUE);
    fElements->SetBit(kMustCleanup); // recursive remove from fCleanups
    gROOT->GetListOfCleanups()->Add(fElements);
  }

  obj->SetBit(kMustCleanup); // recursive remove from fElements and fgAllElements
  fElements->AddAtAndExpand(obj, idx);
  fgAllElements->Add(obj);
  return idx;
}
//______________________________________________________________________________
TObject *TRsnFragment::FindElement(const char *tag) const
{
  // fast find a unique element using its unique tag

  if (!fGroup || !fElements) return 0;
  Int_t idx = fGroup->FindElementTag(tag);
  if (idx < 0) return 0;
  return fElements->UncheckedAt(idx);
}
//______________________________________________________________________________
const char *TRsnFragment::FindElement(const TObject *obj) const
{
  if (!obj) return ""; // must be, otherwise return tag of the first empty slot of fElements
  if (!fGroup || !fElements) return "";
  Int_t idx = fElements->IndexOf(obj);
  if (idx < 0) return "";
  return fGroup->FindElementTag(idx);
}
