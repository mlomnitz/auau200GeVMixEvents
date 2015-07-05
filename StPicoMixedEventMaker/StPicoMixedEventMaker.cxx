#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

#include "StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoPrescales/StPicoPrescales.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoMixedEventMaker.h"
#include "StPicoEventMixer.h"
#include "StRoot/StRefMultCorr/StRefMultCorr.h"
#include "StRoot/StEventPlane/StEventPlane.h"

#include <vector>

ClassImp(StPicoMixedEventMaker)

// _________________________________________________________
StPicoMixedEventMaker::StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, StRefMultCorr* grefmultCorrUtil, StEventPlane* eventPlaneMaker,
      char const* outputBaseFileName,  char const* inputHFListHFtree = "") :
   StMaker(name), mPicoDst(NULL), mPicoDstMaker(picoMaker),  mPicoEvent(NULL),
   mGRefMultCorrUtil(grefmultCorrUtil), mEventPlane(eventPlaneMaker), mPicoEventMixer(NULL),
   mOuputFileBaseName(outputBaseFileName), mInputFileName(inputHFListHFtree),
   mEventCounter(0), mTree(NULL), mOutputFileTree(NULL)
{

   mOutputFileTree = new TFile(Form("%s.picoMEtree.root", mOuputFileBaseName.Data()), "RECREATE");
   mOutputFileTree->SetCompressionLevel(1);
   mOutputFileTree->cd();
   // -- create OutputTree
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   if (!mTree)
      mTree = new TTree("T", "T", BufSize);
   mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
   //  mTree->Branch("mixedEvent", "StPicoMixedEvent", &mPicoEventMixer, BufSize, Split);

   // -- constructor
}

// _________________________________________________________
StPicoMixedEventMaker::~StPicoMixedEventMaker()
{
   delete mGRefMultCorrUtil;
}
// Method should load Q vector stuff from Hao, needs fixing
// _________________________________________________________
bool StPicoMixedEventMaker::loadEventPlaneCorr(StEventPlane const * mEventPlane)
{
   //needs to implement, will currently break maker
   if(!mEventPlane)
   {
      LOG_WARN << "No EventPlane ! Skipping! " << endm;
      return kFALSE;
   }
   // cout<<"mEventPlane->getAcceptEvent()="<<mEventPlane->getAcceptEvent()<<endl;
   if(!mEventPlane->getAcceptEvent()) {
      LOG_WARN << "StPicoMixedEvent::THistograms and TProiles NOT found! shoudl check the input Qvector files From HaoQiu ! Skipping this run! " << endm;
     return kFALSE;
   }
   return kTRUE;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Init()
{
   mOutputFileTree->cd();
   mPicoEventMixer = new StPicoEventMixer("cat0");
   mPicoEventMixer->setEventBuffer(10);
   mGRefMultCorrUtil = new StRefMultCorr("grefmult");


   // -- reset event to be in a defined state
   //resetEvent();

   return kStOK;
}

// _________________________________________________________
Int_t StPicoMixedEventMaker::Finish()
{
   // -- Inhertited from StMaker
   //    NOT TO BE OVERWRITTEN by daughter class
   //    daughter class should implement FinishHF()
   mOutputFileTree->cd();
   mPicoEventMixer->finish();
   mOutputFileTree->Write();
   mOutputFileTree->Close();

   //mOutputFileList->cd();
   //mOutputFileList->Write(mOutputFileList->GetName(), TObject::kSingleKey);

   //mOutputFileList->Close();

   return kStOK;
}
// _________________________________________________________
void StPicoMixedEventMaker::Clear(Option_t* opt)
{
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Make()
{

   if (!mPicoDstMaker)
   {
      LOG_WARN << "No PicoDstMaker! Skipping! " << endm;
      return kStWarn;
   }

   StPicoDst const* picoDst = mPicoDstMaker->picoDst();
   if (!picoDst)
   {
      LOG_WARN << "No picoDst ! Skipping! " << endm;
      return kStWarn;
   }
   // - GRef from Guannan
   if (!mGRefMultCorrUtil)
   {
      LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
      return kStWarn;
   }
   StThreeVectorF const pVtx = picoDst->event()->primaryVertex();
   mGRefMultCorrUtil->init(picoDst->event()->runId());
   mGRefMultCorrUtil->initEvent(picoDst->event()->grefMult(), pVtx.z(), picoDst->event()->ZDCx()) ;
   int centrality  = mGRefMultCorrUtil->getCentralityBin9();
//     4            55-60%            30-40%
//     5            50-55%            20-30%
//     6            45-50%            10-20%

   if(!loadEventPlaneCorr(mEventPlane)){
   LOG_WARN << "Event plane calculations unavalable! Skipping"<<endm;
   return kStOK;
   // continue;
   }

//   cout<<"GUANNAN Xie Check==========="<<endl;
//   cout<<",yEventPlane->getRunId()="<<mEventPlane->getRunId()<<endl;
//   cout<<"RunId()="<<picoDst->event()->runId()<<endl;
//   cout<<"getCentrality()="<<centrality<<endl;
//   cout<<"mEventPlane->getCentrality()="<<mEventPlane->getCentrality()<<endl;
//   cout<<"mEventPlane->getEventPlane()="<<mEventPlane->getEventPlane()<<endl;

   if (centrality < 4 || centrality > 6) return kStOk;
   //cout<<"Centrality: "<<centrality<<endl;
   // - - -
   if (mPicoEventMixer -> addPicoEvent(picoDst) ==  true)
      mPicoEventMixer->mixEvents();
   //mTree->Fill();

   return kStOk;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::SetCategories()
{
   return kStOk;
}
// _________________________________________________________
int StPicoMixedEventMaker::categorize(StPicoDst const * picoDst)
{
   StThreeVectorF pVertex = (picoDst->event())->primaryVertex();
   if (fabs(pVertex.z()) > 6.0) return -99;
   int bin = -6.0 + (pVertex.z() + 6.0) / 1.2;
   return bin;
}
