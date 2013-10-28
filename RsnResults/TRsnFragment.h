// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-28

#ifndef RSNFRAGMENT_H
#define RSNFRAGMENT_H

#include <TNamed.h>

class TObjArray;
class TH1;

class TRsnGroup;

class TRsnFragment: public TNamed {

public:
  TRsnFragment();
  virtual ~TRsnFragment();

  Double_t      GetZoneMin() const { return fZoneMin; }
  Double_t      GetZoneMax() const { return fZoneMax; }
  Double_t      GetZoneMean() const { return (fZoneMin+fZoneMax)/2.0; }
  Double_t      GetZoneWidth() const { return TMath::Abs(fZoneMax-fZoneMin); }
  TRsnGroup    *GetGroup() const { return fGroup; }
  TObjArray    *GetListOfElements() const { return fElements; }

  virtual Int_t  Compare(const TObject *obj) const;
  virtual Bool_t IsSortable() const { return kTRUE; }
  virtual void   Print(Option_t *option = "") const;

  void          AddElement(TObject *obj, const char *label);
  TObject      *GetElement(const char *label) const;

private:
  Double_t      fZoneMin;
  Double_t      fZoneMax;
  TRsnGroup    *fGroup;        // !
  TObjArray    *fElements;     // !

  ClassDef(TRsnFragment, 1) // RsnFragment class
};

#endif
