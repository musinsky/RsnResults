// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-01-27

#include <THnSparse.h>
#include <TH1.h>

#include "TRsnSparseHandler.h"
#include "TRsnGroup.h"
#include "TRsnFragment.h"
#include "TRsnUtils.h"

ClassImp(TRsnSparseHandler)

//______________________________________________________________________________
TRsnSparseHandler::TRsnSparseHandler(const THnBase *sparse)
: TObject(),
  fGroup(0),
  fAxes(0),
  fBinFragments(0),
  fDimFragments(-1),
  fDimElement(-1)
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
    fAxes->AddAt(dummy, dim);
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
    if (dim == fDimFragments) printf("\t<= Fragments");
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
  Printf("number of fragments = %d", size);
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    low = dummy->GetBinLowEdge(first);
    up  = dummy->GetBinUpEdge(last);
    printf("fragment[%03d]\t[%03d, %03d]\t(%f, %f)\n", i, first, last, low, up);
  }
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
    Printf("axis[%d]('%s') change previous type Range", dim, dummy->GetName());
    dummy->SetRange(0, 0);
  }
  if (dim == fDimElement) {
    Printf("axis[%d]('%s') change previous type Element", dim, dummy->GetName());
    fDimElement = -1;
  }
  if (dim == fDimFragments) {
    Printf("axis[%d]('%s') change previous type Fragments", dim, dummy->GetName());
    fDimFragments = -1;
    SafeDelete(fBinFragments);
  }

  return dummy;
}
//______________________________________________________________________________
void TRsnSparseHandler::SetAxisFragments(Int_t dim, const TArrayI &array)
{
  TAxis *dummy = CheckAxisType(dim);
  if (!dummy) return;

  if (fDimFragments != -1)
    Printf("axis[%d]('%s') reset previous type Fragments", fDimFragments, GetAxis(fDimFragments)->GetName());

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
    Printf("axis[%d]('%s') reset previous type Element", fDimElement, GetAxis(fDimElement)->GetName());

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
  // check fragments
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

  // make group of fragments
  //  SafeDelete(fGroup); // potrebne (delete vsetky fragmnts and elements) ?!
  // TODO if (fGroup) ????
  //
  // gname = "PT" // !!! a zarove funkcia TryRename (z PT urobi latex p_t a jednotky ako title)
  // samotne fragmenty budu generovat meno a title z group name a title
  fGroup = new TRsnGroup(gname, gtitle);
  TRsnFragment *fragment;
  SetBit(kLockAxes); // TODO alebo lockovat az po pridani elementoch ?! t.j. aj range alebo extra lock na range ?

  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    fragment = fGroup->MakeFragment(dummy->GetBinLowEdge(first), dummy->GetBinUpEdge(last));
    Printf("([%03d] = %03d, [%03d] = %03d)  %d \t (%f, %f) %f  \t %.4f", i, first, size+i, last, last-first,
           fragment->GetMin(), fragment->GetMax(), fragment->GetMean(), fragment->GetWidth());
  }

  return fGroup;
}
//______________________________________________________________________________
void TRsnSparseHandler::AddFragmentElement(const THnBase *sparse, const char *tag) const
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

  // compare dummy and sparse axes
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

  // range
  for (Int_t dim = 0; dim < fAxes->GetSize(); dim++) {
    axis  = sparse->GetAxis(dim);
    dummy = GetAxis(dim);
    if (dummy->TestBit(TAxis::kAxisRange))
      axis->SetRange(dummy->GetFirst(), dummy->GetLast()); // apply axis range
    else
      axis->SetRange(0, 0);                                // reset axis range
  }

  Bool_t save = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  TRsnFragment *fragment;
  Int_t first, last, size = fBinFragments->GetSize()/2;
  axis = sparse->GetAxis(fDimFragments);
  for (Int_t i = 0; i < size; i++) {
    first = fBinFragments->At(i);
    last  = fBinFragments->At(size+i);
    axis->SetRange(first, last); // range for current fragment

    fragment = fGroup->FragmentAt(i);
    if (!fragment) continue;
    TH1 *histo = sparse->Projection(fDimElement, "E");

    Printf("%s, %s", histo->GetName(), sparse->GetName());
    // TODO TryRename histo
    // histo->SetTitle("p_t (min,max)");
    fragment->AddElement(histo, tag ? tag : sparse->GetName());
  }

  Printf("pridal som %d elementov", size);
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
    Int_t binMin = axis->FindFixBin(fragment->GetMean() - 0.49*fragment->GetWidth());
    Int_t binMax = axis->FindFixBin(fragment->GetMean() + 0.49*fragment->GetWidth());
    if ((binMin == 0) || (binMin == (axis->GetNbins() + 1))) return kFALSE; // under/over
    if ((binMax == 0) || (binMax == (axis->GetNbins() + 1))) return kFALSE;
    if (!TRsnUtils::AreEqual(axis->GetBinLowEdge(binMin), fragment->GetMin(), precision)) return kFALSE;
    if (!TRsnUtils::AreEqual(axis->GetBinUpEdge(binMax), fragment->GetMax(), precision)) return kFALSE;
  }

  return kTRUE;
}
