// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-25

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
////______________________________________________________________________________
//void TRsnFragment::AddHisto(TH1 *h, EPairKind kind, EPairType type)
//{
//  if (!h) return;
//  TObjArray *harray = GetHistoArray(kind, kTRUE);
//  if (!harray) return;
//
//  TObject *prev = harray->At(type);
//  if (prev) Warning("AddHisto", "histo %s replaced by %s", prev->GetName(), h->GetName());
//
//  TString name;
//  switch (type) {
//    case kUnlike:   name = "unlike";   break;
//    case kUnlikePM: name = "unlikePM"; break;
//    case kUnlikeMP: name = "unlikeMP"; break;
//    case kLike:     name = "like";     break;
//    case kLikePP:   name = "likePP";   break;
//    case kLikeMM:   name = "likeMM";   break;
//    default:        name = "nan";      break;
//  }
//
//  h->SetName(Form("%s_%s", harray->GetName(), name.Data()));
//  h->SetDirectory(0);
//  h->SetBit(kMustCleanup);
//  harray->AddAt(h, type);
//}
////______________________________________________________________________________
//TH1 *TRsnFragment::GetHisto(EPairKind kind, EPairType type)
//{
//  TObjArray *harray = GetHistoArray(kind, kFALSE);
//  if (!harray) return 0;
//  return (TH1 *)harray->At(type);
//}
////______________________________________________________________________________
