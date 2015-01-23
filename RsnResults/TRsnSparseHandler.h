// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-01-23

#ifndef RSNSPARSEHANDLER_H
#define RSNSPARSEHANDLER_H

#include <TObjArray.h>

class THnBase;
class TAxis;
class TArrayI;
class TRsnGroup;

class TRsnSparseHandler : public TObject {

public:
  // locking axes
  enum { kLockAxes = BIT(14) };

  TRsnSparseHandler(const THnBase *sparse);
  virtual ~TRsnSparseHandler();

  TRsnGroup    *GetGroup() const { return fGroup; }
  TObjArray    *GetAxes() const { return fAxes; }
  TArrayI      *GetBinFragments() const { return fBinFragments; }

  virtual void  Print(Option_t *option = "") const;

  void          SetAxisFragments(Int_t dim, const TArrayI &array);
  void          SetAxisFragments(const char *aname, const TArrayI &array) { SetAxisFragments(GetAxisDim(aname), array); }
  void          SetAxisElement(Int_t dim);
  void          SetAxisElement(const char *aname) { SetAxisElement(GetAxisDim(aname)); }
  void          SetAxisRange(Int_t dim, Int_t first, Int_t last);
  void          SetAxisRange(const char *aname, Int_t first, Int_t last) { SetAxisRange(GetAxisDim(aname), first, last); }
  void          SetAxisRangeUser(Int_t dim, Double_t ufirst, Double_t ulast);
  void          SetAxisRangeUser(const char *aname, Double_t ufirst, Double_t ulast) { SetAxisRangeUser(GetAxisDim(aname), ufirst, ulast); }


  TRsnGroup    *MakeGroupFragments(const char *gname, const char *gtitle = "");
  void          AddFragmentElement(const THnBase *sparse, const char *tag = 0) const;


  // private ?!
  static Bool_t CheckConsistentFragments(TRsnGroup *group, const TAxis *axis);


private:
  TRsnGroup    *fGroup;        //  current group
  TObjArray    *fAxes;         //  dummy axes from sparse
  TArrayI      *fBinFragments; //  first and last bin for each fragment
  Int_t         fDimFragments; //  fragments axis
  Int_t         fDimElement;   //  element axis

  TAxis        *GetAxis(Int_t dim) const { return fAxes ? (TAxis *)fAxes->At(dim) : 0; }
  Int_t         GetAxisDim(const char *name) const { return fAxes ? fAxes->IndexOf(fAxes->FindObject(name)) : -1; }
  TAxis        *CheckAxisType(Int_t dim);

  ClassDef(TRsnSparseHandler, 1) // RsnSparseHandler class
};

#endif
