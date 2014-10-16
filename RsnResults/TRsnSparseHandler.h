// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-10-16

#ifndef RSNSPARSEHANDLER_H
#define RSNSPARSEHANDLER_H

#include <TObject.h>
#include <TObjArray.h>

class THnBase;
class TAxis;
class TRsnGroup;

class TRsnSparseHandler : public TObject {

public:
  TRsnSparseHandler();
  virtual ~TRsnSparseHandler();

  //  virtual void  Print(Option_t *option = "") const;


  TRsnGroup *MakeGroup(const THnBase *hb);

  Bool_t        AddFriend(const THnBase *hb);
  //void          AddCutRange(const char *axis, Double_t first, Double_t last);
  //  Int_t         AddCutRangeUser(const char *axis, Int_t ufirst, Int_t ulast);
  //  Int_t         AddCutRange(Int_t axis, Double_t first, Double_t last);
  //  Int_t         AddCutRangeUser(Int_t dim, Int_t ufirst, Int_t ulast);
  //void          ResetCutRange();


  TAxis        *GetAxis(Int_t dim) const { return fAxes ? (TAxis *)fAxes->At(dim) : 0; }
  TAxis        *GetAxis(const char *name) const { return fAxes ? (TAxis *)fAxes->FindObject(name) : 0; }
  // private ?!
  static Bool_t CheckEqualAxes(const TAxis *a1, const TAxis *a2);
  static Bool_t CheckConsistentFragments(TRsnGroup *group, const TAxis *axis);
  void          DummyAxes(const THnBase *hb);

private:
  TObjArray    *fAxes;    //  dummy axes from sparse
  //  TList        *fCuts;    //  list of all cuts on axes

  ClassDef(TRsnSparseHandler, 1) // RsnSparseHandler class
};

#endif
