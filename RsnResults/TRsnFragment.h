// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-30

#ifndef RSNFRAGMENT_H
#define RSNFRAGMENT_H

#include <TObject.h>

class TObjArray;

class TRsnGroup;

class TRsnFragment: public TObject {

public:
  TRsnFragment();
  TRsnFragment(Double_t min, Double_t max, TRsnGroup *group);
  virtual ~TRsnFragment();

  Double_t       GetMin() const { return fMin; }
  Double_t       GetMax() const { return fMax; }
  Double_t       GetMean() const { return (fMin+fMax)/2.0; }
  Double_t       GetWidth() const { return TMath::Abs(fMax-fMin); }
  TRsnGroup     *GetGroup() const { return fGroup; }
  TObjArray     *GetListOfElements() const { return fElements; }

  virtual Int_t  Compare(const TObject *obj) const;
  virtual Bool_t IsSortable() const { return kTRUE; }
  virtual void   Print(Option_t *option = "") const;

  void           AddElement(TObject *obj, const char *tag);
  TObject       *FindElement(const char *tag) const;
  const char    *FindTag(const TObject *obj) const;

private:
  Double_t       fMin;
  Double_t       fMax;
  TRsnGroup     *fGroup;        // !
  TObjArray     *fElements;     // !

  ClassDef(TRsnFragment, 1) // RsnFragment class
};

#endif
