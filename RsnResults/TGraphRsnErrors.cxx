// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-08-12

#include <TPad.h>
#include <TMarker.h>

#include "TGraphRsnErrors.h"

Bool_t TGraphRsnErrors::fgAroundFlash = kTRUE;

ClassImp(TGraphRsnErrors)

//______________________________________________________________________________
TGraphRsnErrors::TGraphRsnErrors()
: TGraphErrors(),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // Default constructor
}
//______________________________________________________________________________
TGraphRsnErrors::TGraphRsnErrors(Int_t n)
: TGraphErrors(n),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TGraphRsnErrors::TGraphRsnErrors(Int_t n, const Float_t *x, const Float_t *y,
                                 const Float_t *ex, const Float_t *ey)
: TGraphErrors(n, x, y, ex, ey),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TGraphRsnErrors::TGraphRsnErrors(Int_t n, const Double_t *x, const Double_t *y,
                                 const Double_t *ex, const Double_t *ey)
: TGraphErrors(n, x, y, ex, ey),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TGraphRsnErrors::TGraphRsnErrors(const char *filename, const char *format, Option_t *option)
: TGraphErrors(filename, format, option),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // Constructor reading input from filename
}
//______________________________________________________________________________
TGraphRsnErrors::~TGraphRsnErrors()
{
  // Destructor
  SafeDelete(fFlashMarker);
}
//______________________________________________________________________________
char *TGraphRsnErrors::GetObjectInfo(Int_t px, Int_t py) const
{
  if (!gPad) return (char*)"";

  static char info[64];
  Float_t x = gPad->PadtoX(gPad->AbsPixeltoX(px));
  Float_t y = gPad->PadtoY(gPad->AbsPixeltoY(py));
  snprintf(info, 64, "x=%g, y=%g", x, y);
  if (!fFlashMarker) return info;

  snprintf(info, 64, "X[ %d ]=%g, Y[ %d ]=%g  |  x=%g, y=%g",
           fFlashPoint, fX[fFlashPoint], fFlashPoint, fY[fFlashPoint], x, y);
  return info;
}
//______________________________________________________________________________
void TGraphRsnErrors::Paint(Option_t *chopt)
{
  TGraphErrors::Paint(chopt);

  if (!fFlashMarker) return;
  fFlashMarker->SetMarkerColor(GetMarkerColor());
  fFlashMarker->SetMarkerStyle(GetMarkerStyle());
  // visibility of dots markers
  if (fFlashMarker->GetMarkerStyle() < 20) fFlashMarker->SetMarkerStyle(20);
  fFlashMarker->SetMarkerSize(GetMarkerSize()*1.75);
  fFlashMarker->SetX(fX[fFlashPoint]);
  fFlashMarker->SetY(fY[fFlashPoint]);
  fFlashMarker->Paint(chopt);
}
//______________________________________________________________________________
Int_t TGraphRsnErrors::DistancetoPrimitive(Int_t px, Int_t py)
{
  Int_t ret = TGraphErrors::DistancetoPrimitive(px, py);
  if (!fFlashMarker) return ret;
  if (ret == 9999) return ret; // point is not in the graph area

  Int_t pxp, pyp, dx, dxy, d, dmax = 50, prevp = fFlashPoint;
  for (Int_t i = 0; i < fNpoints; i++) {
    pxp = gPad->XtoAbsPixel(gPad->XtoPad(fX[i]));
    pyp = gPad->YtoAbsPixel(gPad->YtoPad(fY[i]));
    dx  = TMath::Abs(pxp-px);
    dxy = TMath::Abs(pyp-py) + dx;
    d   = (fgAroundFlash) ? dxy : dx;
    if (d < dmax) {
      dmax = d;
      fFlashPoint = i;
    }
  }

  if (fFlashPoint != prevp) FlashPoint(kTRUE);
  return ret;
}
//______________________________________________________________________________
void TGraphRsnErrors::AroundFlash(Bool_t around)
{
  // Set flag to finding flash point around (XY) or along (X) graph point

  // static function
  fgAroundFlash = around;
}
//______________________________________________________________________________
Bool_t TGraphRsnErrors::IsAroundFlash()
{
  // static function
  return fgAroundFlash;
}
//______________________________________________________________________________
void TGraphRsnErrors::SetShowFlash(Bool_t set, Option_t *option)
{
  if (!set) {
    FlashPoint(kFALSE, option);
    return;
  }
  if (fFlashMarker) {
    Flash(option); // only to change option in derived class
    return;
  }

  fFlashPoint = 0; // starting with first point from graph
  FlashPoint(kTRUE, option);
}
//______________________________________________________________________________
void TGraphRsnErrors::FlashPoint(Bool_t flash, Option_t *option)
{
  if (!flash) {
    if (fFlashMarker) {
      delete fFlashMarker;
      fFlashMarker = 0;
      fFlashPoint = 0;
    }
    Flash(option); // flash disabled
  }
  else {
    if (!fFlashMarker) {
      fFlashMarker = new TMarker(fX[fFlashPoint], fY[fFlashPoint], GetMarkerStyle());
      fFlashMarker->SetBit(kCannotPick);
      //      fFlashMarker->Draw(); // don't need to add to gPad primitives
    }
    Flash(option); // flash enabled
  }

  if (gPad) {
    gPad->Update();
    gPad->Modified();
  }
}
//______________________________________________________________________________
void TGraphRsnErrors::Flash(Option_t * /*option*/)
{
  // This method must be overridden if an object has to react to Flash

  // AbstractMethod("Flash");
}
