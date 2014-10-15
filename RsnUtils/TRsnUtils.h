// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-10-15

#ifndef RSNUTILS_H
#define RSNUTILS_H

#include <TMath.h>
#include <TArrayI.h>

class TH1;

namespace TRsnUtils {

  inline Bool_t   AreEqual(Double_t a, Double_t b, Bool_t isdouble = kTRUE);
  inline Double_t CheckFinite(Double_t value, Double_t replace);

  TArrayI        RangeFragments(const TH1 *h, Double_t range, Double_t min = 0., Double_t max = 0.,
                                Bool_t round = kFALSE);
  void           RangeFragmentsAdd(const TH1 *h, TArrayI &array, Double_t range, Double_t max);
  void           RangeFragmentsPrint(const TH1 *h, const TArrayI array);
}

inline Bool_t TRsnUtils::AreEqual(Double_t a, Double_t b, Bool_t isdouble)
{
  Double_t epsilon = TMath::Limits<Float_t>::Epsilon();
  if (isdouble) epsilon = TMath::Limits<Double_t>::Epsilon();
  Double_t max = TMath::Max(TMath::Abs(a), TMath::Abs(b));
  if (max > 1.0) epsilon = epsilon*max;
  return TMath::AreEqualAbs(a, b, epsilon);
}

inline Double_t TRsnUtils::CheckFinite(Double_t value, Double_t replace)
{
  if (TMath::Finite(value)) return value;
  return replace;
}

#endif
