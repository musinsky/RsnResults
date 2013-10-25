// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-10-25

#ifndef RSNFRAGMENT_H
#define RSNFRAGMENT_H

#include <TNamed.h>

class TObjArray;
class TH1;
class TRsnGroup;

class TRsnFragment: public TNamed {

public:
  enum EPairKind { kSingle, kMix, kTrueMC, kGenMC };
  enum EPairType { kUnlike = 0, kUnlikePM = 1, kUnlikeMP = 2, kLike = 3, kLikePP = 4, kLikeMM = 5 };

  TRsnFragment();
  virtual ~TRsnFragment();

  Double_t      GetZoneMin() const { return fZoneMin; }
  Double_t      GetZoneMax() const { return fZoneMax; }
  Double_t      GetZoneMean() const { return (fZoneMin+fZoneMax)/2.0; }
  Double_t      GetZoneWidth() const { return TMath::Abs(fZoneMax-fZoneMin); }
  TRsnGroup    *GetGroup() const { return fGroup; }

  virtual Int_t  Compare(const TObject *obj) const;
  virtual Bool_t IsSortable() const { return kTRUE; }
  virtual void   Print(Option_t *option = "") const;

  void          AddHisto(TH1 *h, EPairKind kind, EPairType type);
  TH1          *GetHisto(EPairKind kind, EPairType type);
  static void   HistoOwner(Bool_t owner) { fgHistoOwner = owner; }
  static Bool_t IsHistoOwner() { return fgHistoOwner; }

private:
  enum { kMaxPairType = 6 };

  Double_t      fZoneMin;
  Double_t      fZoneMax;
  TRsnGroup    *fGroup;         // !
  TObjArray    *fHistoSingle;   // !
  TObjArray    *fHistoMix;      // !
  TObjArray    *fHistoTrueMC;   // !
  TObjArray    *fHistoGenMC;    // !
  static Bool_t fgHistoOwner;   // !

  TObjArray    *CreateHistoArray(const char *name, Int_t max);
  TObjArray    *GetHistoArray(EPairKind kind, Bool_t create = kFALSE);

  ClassDef(TRsnFragment, 1) // RsnFragment class
};

#endif
