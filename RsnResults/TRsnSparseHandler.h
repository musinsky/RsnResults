// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-10-15

#ifndef RSNSPARSEHANDLER_H
#define RSNSPARSEHANDLER_H

#include <TObject.h>

class THnBase;
class TAxis;
class TRsnGroup;

class TRsnSparseHandler : public TObject {

public:
  TRsnSparseHandler();
  virtual ~TRsnSparseHandler();

  Bool_t        AddFriend(const THnBase *hb);
  //  Int_t         AddCutRange(const char *axis, Double_t first, Double_t last);
  //  Int_t         AddCutRangeUser(const char *axis, Int_t ufirst, Int_t ulast);
  //  Int_t         AddCutRange(Int_t axis, Double_t first, Double_t last);
  //  Int_t         AddCutRangeUser(Int_t dim, Int_t ufirst, Int_t ulast);

  // private ?!
  static Bool_t CheckEqualAxes(const TAxis *a1, const TAxis *a2);
  static Bool_t CheckConsistentFragments(TRsnGroup *group, const TAxis *axis);
  void          DummyAxes(const THnBase *hb);

private:
  TObjArray    *fAxes;    //  dummy axes of the histogram
  //  TList        *fCuts;    //  list of all cuts on axes

  ClassDef(TRsnSparseHandler, 1) // RsnSparseHandler class
};

#endif
