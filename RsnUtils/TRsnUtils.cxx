// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-11-04

#include <TH1.h>
#include <TSystem.h>

#include "TRsnUtils.h"

//______________________________________________________________________________
TArrayI TRsnUtils::RangeFragments(const TH1 *h, Double_t range, Double_t min, Double_t max,
                                  Bool_t round)
{
  if (!h) return 0;
  TAxis *axis = h->GetXaxis();

  Int_t bmin = 1;
  Int_t bmax = axis->GetNbins();
  if (max > min) {
    // without under/overflow
    bmin = TMath::Max(bmin, axis->FindFixBin(min)); // include min
    bmax = TMath::Min(bmax, axis->FindFixBin(max)); // exclude max
  }

  Int_t first[axis->GetNbins()];
  Int_t last[axis->GetNbins()];
  Int_t count = 0;

  if ((range < 0) || (range > (axis->GetXmax()-axis->GetXmin()))) { // special case
    if (bmin > bmax) return 0;
    first[0] = bmin; last[0] = bmax; count = 1;
    bmin = bmax + 1; // skipping loop bellow
  }

  for (Int_t ib = bmin; ib <= bmax; ib++) {
    Double_t width = axis->GetBinWidth(ib);
    Int_t nwidth = ib;
    while ((width < range) && !TRsnUtils::AreEqual(width, range)) // don't compare directly
      width += axis->GetBinWidth(++nwidth);

    if (round) // rounding range
      if ((width-range) > (axis->GetBinWidth(nwidth)/2.0)) nwidth -= 1;
    nwidth -= ib;
    if (nwidth < 0) nwidth = 0; // minimum possible range (to same bin)

    if ((ib+nwidth) > bmax) break;
    first[count] = ib;
    last[count]  = ib + nwidth;
    count++;
    ib += nwidth;
  }

  TArrayI array(2*count);
  for (Int_t i = 0; i < count; i++) {
    array[i]       = first[i];
    array[count+i] = last[i];
  }
  return array;
}
//______________________________________________________________________________
void TRsnUtils::RangeFragmentsAdd(const TH1 *h, TArrayI &array, Double_t range, Double_t max)
{
  if (!h) return;
  TAxis *axis = h->GetXaxis();

  Int_t size = array.GetSize()/2;
  Double_t min = (size) ? axis->GetBinUpEdge(array[2*size-1]) : axis->GetXmin();
  if (min >= max) {
    Warning("TRsnUtils::RangeFragmentsAdd", "max %f must be greater than %f", max, min);
    return;
  }

  TArrayI arrayAdd = TRsnUtils::RangeFragments(h, range, min, max, kFALSE);
  Int_t sizeAdd = arrayAdd.GetSize()/2;
  if (!sizeAdd) {
    Warning("TRsnUtils::RangeFragmentsAdd", "no added fragment");
    return;
  }
  if (size && ((arrayAdd[0] - array[2*size-1]) != 1)) {
    Warning("TRsnUtils::RangeFragmentsAdd", "discontiguous fragment");
    return;
  }

  TArrayI arrayMerge(size*2 + sizeAdd*2);
  Int_t sizeMerge = arrayMerge.GetSize()/2;
  for (Int_t i = 0; i < size; i++) {
    arrayMerge[i]           = array[i];
    arrayMerge[sizeMerge+i] = array[size+i];
  }
  for (Int_t i = 0; i < sizeAdd; i++) {
    arrayMerge[size+i]           = arrayAdd[i];
    arrayMerge[sizeMerge+size+i] = arrayAdd[sizeAdd+i];
  }
  array = arrayMerge;
}
//______________________________________________________________________________
void TRsnUtils::RangeFragmentsPrint(const TH1 *h, const TArrayI array)
{
  if (!h) return;
  TAxis *axis = h->GetXaxis();

  Int_t size = array.GetSize()/2;
  Int_t first, last = 0;
  Double_t low, up;
  for (Int_t i = 0; i < size; i++) {
    first = array[i];
    if (i && ((first-last) != 1))
      Error("TRsnUtils::RangeFragmentsPrint", "array must be contiguous");
    last  = array[size+i];
    low   = axis->GetBinLowEdge(first);
    up    = axis->GetBinUpEdge(last);
    Printf("([%03d] = %03d, [%03d] = %03d)  %d \t (%f, %f) %f  \t %.4f", i, first, size+i, last,
           last-first, low, up, (low+up)/2.0, up-low);
  }
}
//______________________________________________________________________________
void TRsnUtils::MemoryInfo()
{
  ProcInfo_t info;
  gSystem->GetProcInfo(&info);

  Printf("virtual = %ld KB (%.1f MiB), resident = %ld KB (%.1f MiB)",
         info.fMemVirtual, info.fMemVirtual/1024.0, info.fMemResident, info.fMemResident/1024.0);
}
//______________________________________________________________________________
Bool_t TRsnUtils::CheckEqualBinAxes(const TAxis *a1, const TAxis *a2)
{
  if (!a1 || !a2) return kFALSE;

  if (a1->GetNbins() != a2->GetNbins()) return kFALSE;

  Bool_t sameFixVar = kTRUE;
  if (a1->IsVariableBinSize() != a2->IsVariableBinSize()) sameFixVar = kFALSE;
  // compare axes with fix and variable bin size must be compare with "low precision"

  if (!TRsnUtils::AreEqual(a1->GetXmin(), a2->GetXmin(), sameFixVar)) return kFALSE;
  if (!TRsnUtils::AreEqual(a1->GetXmax(), a2->GetXmax(), sameFixVar)) return kFALSE;
  // identical axes with fix bin size
  if (!a1->IsVariableBinSize() && sameFixVar) return kTRUE;

  for (Int_t ib = 1; ib <= a1->GetNbins(); ib++)
    if (!TRsnUtils::AreEqual(a1->GetBinLowEdge(ib), a2->GetBinLowEdge(ib), sameFixVar)) return kFALSE;

  if (!sameFixVar) Info("TRsnUtils::CheckEqualBinAxes", "same axes with fix and variable bin size");
  // else identical axes with variable bin size
  return kTRUE;
}
