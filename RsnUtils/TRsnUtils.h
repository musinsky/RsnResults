// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-11-19

#ifndef RSNUTILS_H
#define RSNUTILS_H

#include <TMath.h>
#include <TArrayI.h>

class TH1;

namespace TRsnUtils {

  inline Bool_t  AreEqual(Double_t a, Double_t b, Double_t eps = 1.E-8);

  TArrayI        RangeFragments(const TH1 *h, Double_t range, Double_t min = 0., Double_t max = 0.,
                                Bool_t round = kFALSE);
  void           RangeFragmentsAdd(const TH1 *h, TArrayI &array, Double_t range, Double_t max);
  void           RangeFragmentsPrint(const TH1 *h, const TArrayI array);
}

inline Bool_t TRsnUtils::AreEqual(Double_t a, Double_t b, Double_t eps) {
  return TMath::AreEqualAbs(a, b, eps);
}

#endif
