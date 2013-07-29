// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2013-07-29

#ifndef RSNRESULTS_H
#define RSNRESULTS_H

#include <TNamed.h>

class TRsnResults : public TNamed {

public:
  TRsnResults();
  TRsnResults(const TRsnResults &copy);
  TRsnResults &operator=(const TRsnResults &other);
  virtual ~TRsnResults();

private:

  ClassDef(TRsnResults, 1) // RsnResults class
};

#endif
