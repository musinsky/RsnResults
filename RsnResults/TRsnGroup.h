// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2015-04-17

#ifndef RSNGROUP_H
#define RSNGROUP_H

#include <TObjArray.h>

class THashList;
class TRsnFragment;

class TRsnGroup : public TNamed {

public:
  TRsnGroup();
  TRsnGroup(const char *name, const char *title = "");
  virtual ~TRsnGroup();

  TObjArray    *GetListOfFragments() const { return fFragments; }
  Int_t         GetNFragments() const { return fFragments ? fFragments->GetEntriesFast() : -1; }

  virtual void  Print(Option_t *option = "") const;

  TRsnFragment *MakeFragment(Double_t min, Double_t max);
  TRsnFragment *FindFragment(Double_t inside) const;
  TRsnFragment *FragmentAt(Int_t i) const { return fFragments ? (TRsnFragment *)fFragments->At(i) : 0; }
  Int_t         AddElementTag(const char *tag);
  Int_t         FindElementTag(const char *tag) const;
  const char   *FindElementTag(Int_t idx) const;

private:
  TObjArray    *fFragments;   // list of fragments
  THashList    *fElementTags; // list of unique element tags

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif
