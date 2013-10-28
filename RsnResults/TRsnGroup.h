// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-28

#ifndef RSNGROUP_H
#define RSNGROUP_H

#include "TGraphRsnErrors.h"

class THashList;

class TRsnFragment;

class TRsnGroup : public TGraphRsnErrors {

public:
  TRsnGroup();
  TRsnGroup(Int_t n);
  TRsnGroup(Int_t n, const Float_t *x, const Float_t *y, const Float_t *ex = 0, const Float_t *ey = 0);
  TRsnGroup(Int_t n, const Double_t *x, const Double_t *y, const Double_t *ex = 0, const Double_t *ey = 0);
  TRsnGroup(const char *filename, const char *format = "%lg %lg %lg %lg", Option_t *option = "");
  virtual ~TRsnGroup();

  TObjArray    *GetListOfFragments() const { return fFragments; }
  THashList    *GetElementLabels() const { return fElementLabels; }
  static TList *GetListOfAllElements();

  virtual void  Flash(Option_t *option = "");

  void          AddAtFragment(TObject* obj, Int_t idx); // temp

  TRsnFragment *MakeFragment(Double_t min, Double_t max);
  const char   *FindElementLabel(Int_t element) const;
  Int_t         FindElementLabel(const char *label) const;

private:
  TObjArray    *fFragments;     // !
  THashList    *fElementLabels; // !
  static TList *fgAllElements;  // !

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif
