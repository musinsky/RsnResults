// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-07-29

#include "TRsnResults.h"

ClassImp(TRsnResults)

//______________________________________________________________________________
TRsnResults::TRsnResults()
: TNamed()
{
  // Default constructor
}
//______________________________________________________________________________
TRsnResults::TRsnResults(const TRsnResults &copy)
: TNamed(copy)
{
  // Copy constructor
}
//______________________________________________________________________________
TRsnResults &TRsnResults::operator=(const TRsnResults &other)
{
  // Assignment operator
  if (this != &other) {
    ;
  }
  return *this;
}
//______________________________________________________________________________
TRsnResults::~TRsnResults()
{
  // Destructor
}
