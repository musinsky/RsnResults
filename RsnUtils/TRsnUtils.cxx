// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-11-19

#include <TH1.h>

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
  for (Int_t ib = bmin; ib <= bmax; ib++) {
    Double_t width = axis->GetBinWidth(ib);
    Int_t nwidth = ib;
    while ((width < range) && !AreEqual(width, range)) // don't compare directly
      width += axis->GetBinWidth(++nwidth);

    if (round) // rounding range
      if ((width-range) > (axis->GetBinWidth(nwidth)/2.0)) nwidth -= 1;
    nwidth -= ib;
    if (nwidth < 0) nwidth = 0; // special case

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
    if (i > 0 && ((first-last) != 1))
      Error("TRsnUtils::RangeFragmentsPrint", "array must be contiguous");
    last  = array[size+i];
    low   = axis->GetBinLowEdge(first);
    up    = axis->GetBinUpEdge(last);
    Printf("([%03d] = %03d, [%03d] = %03d)  %d  (%f, %f) %f   \t %.4f", i, first, size+i, last,
           last-first, low, up, (low+up)/2.0, up-low);
  }
}
