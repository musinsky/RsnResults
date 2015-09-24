// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-22

#include <THnBase.h>
#include <TH1.h>

#include "TRsnSparseHandler.h"
#include "TRsnGroup.h"
#include "TRsnFragment.h"
#include "TRsnUtils.h"

ClassImp(TRsnSparseHandler)

//______________________________________________________________________________
TRsnSparseHandler::TRsnSparseHandler(const THnBase *sparse)
: TObject(),
  fAxes(0),
  fDimFragments(-1),
  fBinFragments(0),
  fDimElement(-1),
  fGroup(0)
{
  // Default constructor
  if (!sparse) return;

  fAxes = new TObjArray(sparse->GetNdimensions());
  fAxes->SetOwner(kTRUE);
  TAxis *axis, *dummy;
  for (Int_t dim = 0; dim < sparse->GetNdimensions(); dim++) {
    axis = sparse->GetAxis(dim);
    if (fAxes->FindObject(axis->GetName()))
      Warning("TRsnSparseHandler", "name '%s' for axes are the same, don't use axis name", axis->GetName());
    if (axis->IsVariableBinSize()) // variable
      dummy = new TAxis(axis->GetNbins(), axis->GetXbins()->GetArray());
    else                           // fix
      dummy = new TAxis(axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
    dummy->SetName(axis->GetName());
    //    fAxes->AddAt(dummy, dim);
    fAxes->Add(dummy);
  }
}
//______________________________________________________________________________
TRsnSparseHandler::~TRsnSparseHandler()
{
  // Destructor
  SafeDelete(fAxes); // also are delete dummy axes
  SafeDelete(fBinFragments);
}
//______________________________________________________________________________
void TRsnSparseHandler::Print(Option_t * /*option*/) const
{
  if (!fAxes) return;

  TAxis *dummy;
  Int_t first, last;
  Double_t low, up;

  // dummy axes info
  Printf("dimension (number of axes) = %d", fAxes->GetSize());
  for (Int_t dim = 0; dim < fAxes->GetSize(); dim++) {
    dummy = GetAxis(dim);
    first = 1;
    last  = dummy->GetNbins();
    low = dummy->GetBinLowEdge(first);
    up  = dummy->GetBinUpEdge(last);

    printf("axis[%d]('%s')\t[%03d, %03d]\t(%f, %f)", dim, dummy->GetName(), first, last, low, up);
    if (dim == fDimFragments) printf("\t<= \033[1mFragments\033[0m");
    if (dim == fDimElement)   printf("\t<= Element");
    if (dummy->TestBit(TAxis::kAxisRange)) {
      first = dummy->GetFirst();
      last  = dummy->GetLast();
      low = dummy->GetBinLowEdge(first);
      up  = dummy->GetBinUpEdge(last);
      printf("\t<= Range [%03d, %03d]\t(%f, %f)", first, last, low, up);
    }
    printf("\n");
  }

  // fragments info
  if ((fDimFragments == -1) || !fBinFragments) return;
  dummy = GetAxis(fDimFragments);
  Int_t size = fBinFragments->GetSize()/2;
  Printf("number of \033[1mFragments\033[0m = %d", size);
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    low = dummy->GetBinLowEdge(first);
    up  = dummy->GetBinUpEdge(last);
    printf("%s[%03d]\t[%03d, %03d]\t(%f, %f)\n", fGroup ? fGroup->GetName() : "fragment",
        i, first, last, low, up);
  }
}
//______________________________________________________________________________
TAxis *TRsnSparseHandler::GetAxis(Int_t dim) const
{
  if (!fAxes) return 0;
  TAxis *axis = (TAxis *)fAxes->At(dim);
  if (!axis) {
    Warning("GetAxis", "axis with dimension %d doesn't exist", dim);
    return 0;
  }
  else return axis;
}
//______________________________________________________________________________
Int_t TRsnSparseHandler::GetAxis(const char *name) const
{
  if (!fAxes) return -1;
  TObject *oaxis = fAxes->FindObject(name);
  if (!oaxis) {
    Warning("GetAxis", "axis with name '%s' doesn't exist", name);
    return -1;
  }
  else return fAxes->IndexOf(oaxis); // oaxis != 0 (no index of first empty slot)
}
//______________________________________________________________________________
TAxis *TRsnSparseHandler::CheckAxisType(Int_t dim)
{
  TAxis *dummy = GetAxis(dim);
  if (!dummy) return 0;

  if (TestBit(kLockAxes)) {
    Warning("CheckAxisType", "all axes are locked");
    return 0;
  }

  if (dummy->TestBit(TAxis::kAxisRange)) {
    Printf("axis[%d]('%s') change from previous type Range", dim, dummy->GetName());
    dummy->SetRange(0, 0);
  }
  if (dim == fDimElement) {
    Printf("axis[%d]('%s') change from previous type Element", dim, dummy->GetName());
    fDimElement = -1;
  }
  if (dim == fDimFragments) {
    Printf("axis[%d]('%s') change from previous type Fragments", dim, dummy->GetName());
    fDimFragments = -1;
    SafeDelete(fBinFragments);
  }

  return dummy;
}
//______________________________________________________________________________
void TRsnSparseHandler::SetAxisFragments(Int_t dim, const TArrayI &array)
{
  if (fGroup) {
    Warning("SetAxisFragments", "group '%s' with fragments already exist", fGroup->GetName());
    return;
  }
  TAxis *dummy = CheckAxisType(dim);
  if (!dummy) return;

  if (fDimFragments != -1)
    Printf("axis[%d]('%s') reset from previous type Fragments", fDimFragments, GetAxis(fDimFragments)->GetName());

  fDimFragments = dim;
  SafeDelete(fBinFragments);
  fBinFragments = new TArrayI(array);
}
//______________________________________________________________________________
void TRsnSparseHandler::SetAxisElement(Int_t dim)
{
  TAxis *dummy = CheckAxisType(dim);
  if (!dummy) return;

  if (fDimElement != -1)
    Printf("axis[%d]('%s') reset from previous type Element", fDimElement, GetAxis(fDimElement)->GetName());

  fDimElement = dim;
}
//______________________________________________________________________________
void TRsnSparseHandler::SetAxisRange(Int_t dim, Int_t first, Int_t last)
{
  TAxis *dummy = CheckAxisType(dim);
  if (!dummy) return;

  dummy->SetRange(first, last);
}
//______________________________________________________________________________
void TRsnSparseHandler::SetAxisRangeUser(Int_t dim, Double_t ufirst, Double_t ulast)
{
  TAxis *dummy = CheckAxisType(dim);
  if (!dummy) return;

  dummy->SetRangeUser(ufirst, ulast);
}
//______________________________________________________________________________
TRsnGroup *TRsnSparseHandler::MakeGroupFragments(const char *gname, const char *gtitle)
{
  // check array (bins) of fragments
  TAxis *dummy = GetAxis(fDimFragments);
  if (!dummy || !fBinFragments) return 0;

  Int_t first, last, size = fBinFragments->GetSize()/2;
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    if (i && ((first-last) != 1)) {
      Error("MakeGroupFragments", "array must be contiguous");
      return 0;
    }
    last  = fBinFragments->At(size+i);
    if ((last < first) || (first < 1) || (last > dummy->GetNbins())) {
      Error("MakeGroupFragments", "bad array range");
      return 0;
    }
  }

  // make group with fragments
  if (fGroup) Warning("MakeGroupFragments", "replacing existing group '%s'", fGroup->GetName());
  fGroup = new TRsnGroup(gname ? gname : dummy->GetName(), gtitle);
  TRsnFragment *fragment;
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    fragment = fGroup->MakeFragment(dummy->GetBinLowEdge(first), dummy->GetBinUpEdge(last));
    Printf("%s[%03d]   [%03d, %03d] |%d|   (%f, %f) |%f| \t %f",
           fGroup->GetName(), i, first, last, last-first,
           fragment->GetMin(), fragment->GetMax(), fragment->GetWidth(), fragment->GetMean());
  }
  return fGroup;
}
//______________________________________________________________________________
void TRsnSparseHandler::AddFragmentElement(const THnBase *sparse, const char *tagname)
{
  if (!sparse || !fAxes) return;
  if (!fGroup || !fBinFragments || (fDimFragments == -1)) {
    Warning("AddFragmentElement", "first must create group of fragments");
    return;
  }
  if (fDimElement == -1) {
    Warning("AddFragmentElement", "axis with element not specified");
    return;
  }

  // check all dummy and sparse axes
  if (sparse->GetNdimensions() != fAxes->GetSize()) {
    Error("AddFragmentElement", "different dimension of sparse");
    return;
  }

  TAxis *axis, *dummy;
  for (Int_t dim = 0; dim < fAxes->GetSize(); dim++) {
    axis  = sparse->GetAxis(dim);
    dummy = GetAxis(dim);
    if (!TRsnUtils::CheckEqualBinAxes(axis, dummy)) {
      Error("AddFragmentElement", "axis[%d]('%s') is incompatible", dim, axis->GetName());
      return;
    }
    TString axisName(axis->GetName());
    if (!axisName.EqualTo(dummy->GetName(), TString::kExact)) {
      Error("AddFragmentElement", "axis[%d]('%s') with different name", dim, axis->GetName());
      return;
    }
  }

  SetBit(kLockAxes); // no more SetAxisSomething

  // range
  for (Int_t dim = 0; dim < fAxes->GetSize(); dim++) {
    axis  = sparse->GetAxis(dim);
    dummy = GetAxis(dim);
    if (dummy->TestBit(TAxis::kAxisRange))
      axis->SetRange(dummy->GetFirst(), dummy->GetLast()); // apply axis range
    else
      axis->SetRange(0, 0);                                // reset axis range
  }

  // element
  Bool_t save = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  Int_t first, last, size = fBinFragments->GetSize()/2;
  TRsnFragment *fragment;
  TH1 *his;
  TString tagName(tagname);
  if (tagName.IsWhitespace())
    tagName = TString::Format("%s_%s", sparse->GetName(), sparse->GetAxis(fDimElement)->GetName());
  axis = sparse->GetAxis(fDimFragments);
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    axis->SetRange(first, last); // current fragment range
    his = sparse->Projection(fDimElement, "E");
    fragment = fGroup->FragmentAt(i);
    fragment->AddElement(his, tagName.Data());
    Printf("%s[%03d]   (%f, %f) \t '%s' '%s' \t %d",
           fGroup->GetName(), i, fragment->GetMin(), fragment->GetMax(),
           tagName.Data(), his->GetName(), (Int_t)his->GetEntries());
    //    Printf("%s", his->GetTitle());
  }

  TH1::AddDirectory(save);
}



//______________________________________________________________________________
Bool_t TRsnSparseHandler::CheckConsistentFragments(const TRsnGroup *group, const TAxis *axis)
{
  // check non identical sprase axis
  if (!group || !axis) return kFALSE;

  TIter next(group->GetListOfFragments());
  TRsnFragment *fragment;
  Bool_t precision = !(axis->IsVariableBinSize());
  while ((fragment = (TRsnFragment *)next())) {
    //    Int_t binMin = axis->FindFixBin(fragment->GetMean() - 0.49*fragment->GetWidth());
    //    Int_t binMax = axis->FindFixBin(fragment->GetMean() + 0.49*fragment->GetWidth());
    Int_t binMin = axis->FindFixBin(fragment->GetMin()+0.00001*fragment->GetWidth());
    Int_t binMax = axis->FindFixBin(fragment->GetMax()-0.0001);
    Printf("%d, %d", binMin, binMax);
    if ((binMin == 0) || (binMin == (axis->GetNbins() + 1))) return kFALSE; // under/over
    if ((binMax == 0) || (binMax == (axis->GetNbins() + 1))) return kFALSE;
    if (!TRsnUtils::AreEqual(axis->GetBinLowEdge(binMin), fragment->GetMin(), precision)) return kFALSE;
    if (!TRsnUtils::AreEqual(axis->GetBinUpEdge(binMax), fragment->GetMax(), precision)) return kFALSE;
  }

  return kTRUE;
}
