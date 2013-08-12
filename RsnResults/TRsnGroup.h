// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-08-12

#ifndef RSNGROUP_H
#define RSNGROUP_H

#include "TGraphRsnErrors.h"

class TRsnGroup : public TGraphRsnErrors {

public:
  TRsnGroup();
  TRsnGroup(Int_t n);
  TRsnGroup(Int_t n, const Float_t *x, const Float_t *y, const Float_t *ex = 0, const Float_t *ey = 0);
  TRsnGroup(Int_t n, const Double_t *x, const Double_t *y, const Double_t *ex = 0, const Double_t *ey = 0);
  TRsnGroup(const char *filename, const char *format = "%lg %lg %lg %lg", Option_t *option = "");
  virtual ~TRsnGroup();

  virtual void  Flash(Option_t *option = "");

  void          AddAtFragment(TObject* obj, Int_t idx);
  TObjArray    *GetListOfFragments() const { return fFragments; }

private:
  TObjArray    *fFragments;

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif
