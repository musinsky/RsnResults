// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-11-12

#include <THnSparse.h>
#include <TH1.h>

#include "TRsnSparseHandler.h"
#include "TRsnGroup.h"
#include "TRsnFragment.h"
#include "TRsnUtils.h"

ClassImp(TRsnSparseHandler)

//______________________________________________________________________________
TRsnSparseHandler::TRsnSparseHandler()
: TObject(),
  fAxes(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnSparseHandler::~TRsnSparseHandler()
{
  // Destructor
  SafeDelete(fAxes); // also are delete dummy axes
}
//______________________________________________________________________________
Bool_t TRsnSparseHandler::AddFriend(const THnBase *hb) // const
{
  if (!hb) return kFALSE;
  if (!fAxes) { // first time
    DummyAxes(hb);
    return kTRUE;
  }

  // compare axes
  if (hb->GetNdimensions() != fAxes->GetSize()) return kFALSE;
  TAxis *axis, *dummy;
  for (Int_t dim = 0; dim < hb->GetNdimensions(); dim++) {
    axis = hb->GetAxis(dim);
    dummy = (TAxis *)fAxes->At(dim);
    if (!CheckEqualAxes(axis, dummy)) return kFALSE;
  }

  return kTRUE;
}
//______________________________________________________________________________
Bool_t TRsnSparseHandler::CheckEqualAxes(const TAxis *a1, const TAxis *a2) // const or static
{
  if (!a1 || !a2) return kFALSE;

  // fix and variable
  if (a1->IsVariableBinSize() != a2->IsVariableBinSize()) return kFALSE;
  if (a1->GetNbins() != a2->GetNbins()) return kFALSE;
  if (!TRsnUtils::AreEqual(a1->GetXmin(), a2->GetXmin(), kTRUE)) return kFALSE;
  if (!TRsnUtils::AreEqual(a1->GetXmax(), a2->GetXmax(), kTRUE)) return kFALSE;
  // check name ?!
  Printf("fix same");
  if (!a1->IsVariableBinSize()) return kTRUE;

  // only variable
  for (Int_t b = 0; b <= a1->GetNbins(); b++)
    if (!TRsnUtils::AreEqual(a1->GetXbins()->At(b), a2->GetXbins()->At(b), kTRUE)) return kFALSE;
  Printf("variable same");
  return kTRUE;
}
//______________________________________________________________________________
Bool_t TRsnSparseHandler::CheckConsistentFragments(TRsnGroup *group, const TAxis *axis)
{
  // TODO need testing
  if (!group || !axis) return kFALSE;

  TIter next(group->GetListOfFragments());
  TRsnFragment *fragment;
  while ((fragment = (TRsnFragment *)next())) {
    Int_t bin = axis->FindFixBin(fragment->GetMean()); // no attempt to rebin
    if ((bin == 0) || (bin == (axis->GetNbins() + 1))) return kFALSE; // under/over
    if (!TRsnUtils::AreEqual(axis->GetBinLowEdge(bin), fragment->GetMin(), kTRUE)) return kFALSE;
    if (!TRsnUtils::AreEqual(axis->GetBinUpEdge(bin), fragment->GetMax(), kTRUE)) return kFALSE;
  }

  return kTRUE;
}
//______________________________________________________________________________
void TRsnSparseHandler::DummyAxes(const THnBase *hb)
{
  if (!hb) return;

  if (fAxes) SafeDelete(fAxes); // also are delete previous dummies
  fAxes = new TObjArray(hb->GetNdimensions());
  fAxes->SetOwner(kTRUE);

  TAxis *axis, *dummy;
  for (Int_t dim = 0; dim < hb->GetNdimensions(); dim++) {
    axis = hb->GetAxis(dim);
    if (axis->IsVariableBinSize()) // variable
      dummy = new TAxis(axis->GetNbins(), axis->GetXbins()->GetArray());
    else                           // fix
      dummy = new TAxis(axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
    dummy->SetName(axis->GetName());
    fAxes->AddAt(dummy, dim);
  }
}



// TODO
//______________________________________________________________________________
//void TRsnSparseHandler::AddCutRange(const char *axis, Double_t first, Double_t last)
//{
//  if (!fAxes) return;
//
//}
//______________________________________________________________________________
TRsnGroup *TRsnSparseHandler::MakeGroup(const THnBase *hb)
{
  if (!hb) return 0;

  Bool_t save = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  // cuts on eta
  hb->GetAxis(2)->SetRangeUser(-0.5, 0.5);

  TRsnGroup *group = new TRsnGroup("meno", "tituolk");
  TRsnFragment *fragment;

  TAxis *axis = hb->GetAxis(1);
  for (Int_t i = 10; i < 20; i++) {
    // checkovat aby to bolo stale > 0 && <= fNbins (alebo GetLast/First)
    axis->SetRange(i, i+1);
    fragment = group->MakeFragment(axis->GetBinLowEdge(axis->GetFirst()),
                                   axis->GetBinUpEdge(axis->GetLast()));
    TH1 *histo = hb->Projection(0);
    fragment->AddElement(histo, "unlike");
    histo->SetTitle("p_t (min,max)");

  }

  TH1::AddDirectory(save);
  return group;
}
