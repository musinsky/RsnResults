// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    2014-11-18

#ifndef RSNGROUP_H
#define RSNGROUP_H

#include <TNamed.h>

class TIter;
class THashList;
class TRsnFragment;
class TGraph;

class TRsnGroup : public TNamed {

public:
  TRsnGroup();
  TRsnGroup(const char *name, const char *title = "");
  virtual ~TRsnGroup();

  TObjArray     *GetListOfFragments() const { return fFragments; }
  //  Int_t          GetNFragments() const {
  //    return (fFragments) ? fFragments->GetEntriesFast() : 0; }
  // protect fElementTags
  const THashList *GetListOfElementTags() const { return fElementTags; } // GetElementTags
  //  Int_t          GetNElementTags() const {
  //    return (fElementTags) ? fElementTags->GetSize() : 0; }

  virtual void   Print(Option_t *option = "") const;

  TRsnFragment  *MakeFragment(Double_t min, Double_t max);
  TRsnFragment  *GetFragment(Double_t inside) const;
  // ResetFragments(), NextFragment(), CurrentFragment()
  Int_t          AddElementTag(const char *tag);
  Int_t          FindElementTag(const char *tag) const;
  const char    *FindElementTag(Int_t idx) const;

  void           DrawHighlightFragments() const;
  void           HighlightFragment(const TGraph *gr) const;

private:
  TObjArray     *fFragments;    //  list of fragments
  THashList     *fElementTags;  //  list of unique element tags

  ClassDef(TRsnGroup, 1) // RsnGroup class
};

#endif
