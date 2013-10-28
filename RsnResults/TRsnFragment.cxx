// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-28

#include <TObjArray.h>
#include <TH1.h>
#include <TROOT.h>

#include "TRsnFragment.h"
#include "TRsnGroup.h"

ClassImp(TRsnFragment)

//______________________________________________________________________________
TRsnFragment::TRsnFragment()
: TNamed(),
  fZoneMin(0.),
  fZoneMax(0.),
  fGroup(0),
  fElements(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnFragment::~TRsnFragment()
{
  SafeDelete(fElements); // objects are deleted (is owner)
}
//______________________________________________________________________________
Int_t TRsnFragment::Compare(const TObject *obj) const
{
  // ToDo: over dynamic_cast
  if (GetZoneMean() < (((TRsnFragment *)obj)->GetZoneMean())) return -1;
  else if (GetZoneMean() > (((TRsnFragment *)obj)->GetZoneMean())) return 1;
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

  if (fGroup->FindElementLabel(label) >=0 ) {
    Error("AddElement", "already exist element label %s in fragment %s", label, GetName());
    return;
  }

  if (!fElements) {
    fElements = new TObjArray();
    fElements->SetName("Elements");
    fElements->SetOwner();
  }
  else if (fElements->Contains(obj)) {
    Error("AddElement", "already exist element %s in fragment %s", obj->GetName(), GetName());
    return;
  }

  fElements->Add(obj); // do not sort this array, must stay this order
  fGroup->SetElementLabel(fElements->GetLast(), label);
}
//______________________________________________________________________________
TObject *TRsnFragment::GetElement(const char *label) const
{
  if (!fGroup || !fElements) return 0;
  Int_t idx = fGroup->FindElementLabel(label);
  if (idx < 0) return 0;
  return fElements->At(idx);
}
