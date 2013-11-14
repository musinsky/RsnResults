// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-11-14

#include <TH1.h>

#include "TRsnUtils.h"

//______________________________________________________________________________
TArrayI *TRsnUtils::RangeFragments(const TH1 *h, Double_t range, Double_t min, Double_t max)
{
  if (!h) return 0;

  TAxis *axis = h->GetXaxis();
  if (axis->IsVariableBinSize()) {
    Error("TRsnUtils::RangeFragments", "x-axis of histo: %s with variable bin size", h->GetName());
    //    return 0;
  }

  Double_t rangemin = axis->GetBinWidth(-1);
  Double_t rangemax = rangemin*axis->GetNbins();
  if      (range < rangemin) range = rangemin;
  else if (range > rangemax) range = rangemax;
  Int_t brange = TMath::Nint(range/rangemin);
  range = brange*rangemin; // nearest multiple of rangemin

  Int_t bmin = 1;
  Int_t bmax = axis->GetNbins();
  if (max > min) {
    // minimum
    min = TMath::Nint(min/range)*range; // nearest multiple of range
    if (min > axis->GetXmin()) {
      bmin = axis->FindFixBin(min);
      if (axis->GetBinUpEdge(bmin) <= min) bmin += 1; // see TAxis::SetRangeUser
    }
    // maximum
    max = TMath::Nint(max/range)*range;
    if (max < axis->GetXmax()) {
      bmax = axis->FindFixBin(max);
      if (axis->GetBinLowEdge(bmax) >= max) bmax -= 1;
    }
  }

  brange -= 1; // important
  if (brange > (bmax - bmin)) {
    Error("TRsnUtils::RangeFragments",
          "range of interval %g is greater than himself interval (%g, %g)", range, min, max);
    return 0;
  }

  Int_t ifirst = bmin;
  Int_t ilast = ifirst + brange;
  Int_t count = 0;
  Int_t tmpFirst[axis->GetNbins()];
  Int_t tmpLast[axis->GetNbins()];
  tmpFirst[count] = ifirst;
  tmpLast[count] = ilast;
  while (ilast < (bmax - brange)) {
    ifirst = ilast + 1;
    ilast = ifirst + brange;
    count++;
    tmpFirst[count] = ifirst;
    tmpLast[count] = ilast;
  }

  count += 1; // from index to size
  TArrayI *array = new TArrayI(2*count);
  for (Int_t i = 0; i < count; i++) {
    array->AddAt(tmpFirst[i], i);
    array->AddAt(tmpLast[i], count + i);
    Double_t le = axis->GetBinLowEdge(array->At(i));
    Double_t ue = axis->GetBinUpEdge(array->At(count + i));
    Printf("[%03d] = %03d, [%03d] = %03d   (%f, %f) %f", i, array->At(i), count + i, array->At(count + i),
           le, ue, (le+ue)/2.0);
  }
  Printf("range = %f   min = %f   max =%f", range, axis->GetBinLowEdge(bmin), axis->GetBinUpEdge(bmax));
  Printf("%f   %f", min, max);
  return array;
}
