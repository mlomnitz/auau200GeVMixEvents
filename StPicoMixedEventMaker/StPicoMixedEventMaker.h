#ifndef StPicoMixedEventMaker_h
#define StPicoMixedEventMaker_h

#include "StMaker.h"

/* **************************************************
 *  Base class for Mixed Event cosntructions
 *
 *  - Usage: Implement specific decay in daughter, i.e. 2 or three body decay
 *
 *  - Methods from StHFCyts utility class can/should be used
 *
 * **************************************************
 *
 *  Initial Authors:
 *            Michael Lomnitz  (mrlomnitz@lbl.gov)
 *            Mustaga Mustafa  (mmustafa@lbl.gov)
 *
 *  ** Code Maintainer
 *
 *
 * **************************************************
 */

class TTree; //Need tod ecide if will be saving TTree, NTuple or histos
class TFile;
class TChain;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StRefMultCorr;
class StEventPlane;
class StD0Hists;
class StPicoEventMixer;
class kfEvent;

class StPicoMixedEventMaker : public StMaker
{
public:
   StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, StRefMultCorr* grefmultCorrUtil, StEventPlane* eventPlaneMaker,
                         char const* outputBaseFileName, char const* inputPicoList, char const* kfFileList);
   virtual ~StPicoMixedEventMaker();
   virtual Int_t Init();
   virtual Int_t Make();
   virtual Int_t Finish();
   virtual void  Clear(Option_t* opt = "");

private:
   StPicoDst*      mPicoDst;
   StPicoDstMaker* mPicoDstMaker;
   StPicoEvent*    mPicoEvent;
   StRefMultCorr* mGRefMultCorrUtil;
   StEventPlane*  mEventPlaneMaker;
   kfEvent* mKfEvent;
   char const *mKfFileList;
   TChain* mKfChain;

   StPicoEventMixer* mPicoEventMixer[10][9][10]; //Needs to be generalized, have vz and centrality

   Int_t           mFailedRunnumber;

   TString         mOuputFileBaseName;
   TString         mInputFileName;

   int             mEventCounter;

   bool loadEventPlaneCorr(StEventPlane const *mEventPlane);
   bool isMinBiasTrigger() const;
   bool isGoodEvent() const;

   TFile*          mOutputFile;
   StD0Hists* mD0Hists;

   ClassDef(StPicoMixedEventMaker, 0)
};

inline bool StPicoMixedEventMaker::isMinBiasTrigger() const
{
  return picoEvent->triggerWord() & mxeCuts::minBiasTrigger;
}
#endif
