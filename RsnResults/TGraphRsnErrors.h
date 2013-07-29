// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-07-29

#ifndef GRAPHRSNERRORS_H
#define GRAPHRSNERRORS_H

#include <TGraphErrors.h>

class TMarker;

class TGraphRsnErrors : public TGraphErrors {

public:
  TGraphRsnErrors();
  TGraphRsnErrors(Int_t n);
  TGraphRsnErrors(Int_t n, const Float_t *x, const Float_t *y, const Float_t *ex = 0, const Float_t *ey = 0);
  TGraphRsnErrors(Int_t n, const Double_t *x, const Double_t *y, const Double_t *ex = 0, const Double_t *ey = 0);
  TGraphRsnErrors(const char *filename, const char *format = "%lg %lg %lg %lg", Option_t *option = "");
  virtual ~TGraphRsnErrors();

  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);
  virtual void  Draw(Option_t *chopt = "");
  virtual char *GetObjectInfo(Int_t px, Int_t py) const;

  void          SetShowHisto(Option_t *option = ""); // *MENU*
  void          ShowHisto(Option_t *option = "") const;
  void          FlashPoint(Bool_t flash);

protected:

private:
  Int_t         fFlashPoint;        // !
  TMarker      *fFlashMarker;       // !

  ClassDef(TGraphRsnErrors, 1) // GraphRsnErrors class
};

#endif
