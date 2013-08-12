// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-08-12

#include "TRsnGroup.h"

ClassImp(TRsnGroup)

//______________________________________________________________________________
TRsnGroup::TRsnGroup()
: TGraphRsnErrors(),
  fFragments(0)
{
  // Default constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n)
: TGraphRsnErrors(n),
  fFragments(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Float_t *x, const Float_t *y,
                     const Float_t *ex, const Float_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(Int_t n, const Double_t *x, const Double_t *y,
                     const Double_t *ex, const Double_t *ey)
: TGraphRsnErrors(n, x, y, ex, ey),
  fFragments(0)
{
  // Normal constructor
}
//______________________________________________________________________________
TRsnGroup::TRsnGroup(const char *filename, const char *format, Option_t *option)
: TGraphRsnErrors(filename, format, option),
  fFragments(0)
{
  // Constructor reading input from filename
}
//______________________________________________________________________________
TRsnGroup::~TRsnGroup()
{
  // Destructor
}
//______________________________________________________________________________
