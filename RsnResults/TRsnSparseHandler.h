// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-22

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

  TObjArray    *GetAxes() const { return fAxes; }
  Int_t         GetDimFragments() const { return fDimFragments; }
  TArrayI      *GetBinFragments() const { return fBinFragments; }
  Int_t         GetDimElement() const { return fDimElement; }
  TRsnGroup    *GetGroup() const { return fGroup; }
  Bool_t        AreAxesLocked() const { return TestBit(kLockAxes); }

  virtual void  Print(Option_t *option = "") const;

  void          SetAxisFragments(Int_t dim, const TArrayI &array);
  void          SetAxisFragments(const char *aname, const TArrayI &array) { SetAxisFragments(GetAxis(aname), array); }
  void          SetAxisElement(Int_t dim);
  void          SetAxisElement(const char *aname) { SetAxisElement(GetAxis(aname)); }
  void          SetAxisRange(Int_t dim, Int_t first, Int_t last);
  void          SetAxisRange(const char *aname, Int_t first, Int_t last) { SetAxisRange(GetAxis(aname), first, last); }
  void          SetAxisRangeUser(Int_t dim, Double_t ufirst, Double_t ulast);
  void          SetAxisRangeUser(const char *aname, Double_t ufirst, Double_t ulast) { SetAxisRangeUser(GetAxis(aname), ufirst, ulast); }



  TRsnGroup    *MakeGroupFragments(const char *gname = 0, const char *gtitle = 0);
  void          AddFragmentElement(const THnBase *sparse, const char *tagname = 0);
  static Bool_t CheckConsistentFragments(const TRsnGroup *group, const TAxis *axis);

private:
  TObjArray    *fAxes;         // dummy axes from sparse
  Int_t         fDimFragments; // axis of fragments
  TArrayI      *fBinFragments; // first and last bin for each fragment
  Int_t         fDimElement;   // axis of element
  TRsnGroup    *fGroup;        // pointer to current group

  TAxis        *GetAxis(Int_t dim) const;
  Int_t         GetAxis(const char *name) const;
  TAxis        *CheckAxisType(Int_t dim);

  ClassDef(TRsnSparseHandler, 1) // RsnSparseHandler class
};

#endif
