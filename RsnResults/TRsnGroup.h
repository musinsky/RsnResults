// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-11-06

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

  TObjArray     *GetListOfFragments() const { return fFragments; }
  const THashList *GetListOfElementTags() const { return fElementTags; }

  virtual void   Flash(Option_t *option = "");
  virtual void   Print(Option_t *option = "") const;

  void           AddAtFragmentBla(TObject *obj, Int_t idx);

  TRsnFragment  *MakeFragment(Double_t min, Double_t max);
  Int_t          AddElementTag(const char *tag);
  Int_t          FindElementTag(const char *tag) const;
  const char    *FindElementTag(Int_t idx) const;

private:
  TObjArray     *fFragments;    // !
  THashList     *fElementTags;  // !

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif