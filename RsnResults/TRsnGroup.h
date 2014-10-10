// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-10-10

#ifndef RSNGROUP_H
#define RSNGROUP_H

#include <TNamed.h>

class THashList;
class TRsnFragment;

class TRsnGroup : public TNamed {

public:
  TRsnGroup();
  TRsnGroup(const char *name, const char *title = "");
  virtual ~TRsnGroup();

  TObjArray     *GetListOfFragments() const { return fFragments; }
  //  Int_t          GetNFragments() const {
  //    return (fFragments) ? fFragments->GetEntriesFast() : 0; }
  THashList     *GetListOfElementTags() const { return fElementTags; } // GetElementTags
  //  Int_t          GetNElementTags() const {
  //    return (fElementTags) ? fElementTags->GetSize() : 0; }

  virtual void   Print(Option_t *option = "") const;

  TRsnFragment  *MakeFragment(Double_t min, Double_t max);
  Int_t          AddElementTag(const char *tag);
  Int_t          FindElementTag(const char *tag) const;
  const char    *FindElementTag(Int_t idx) const;

private:
  TObjArray     *fFragments;    //! list of fragments
  THashList     *fElementTags;  //! list of unique element tags

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif
