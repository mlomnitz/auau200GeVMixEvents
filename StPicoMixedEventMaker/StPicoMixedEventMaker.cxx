#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"


#include "StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoPrescales/StPicoPrescales.h"
#include "StPicoMixedEventMaker.h"
#include "StPicoEventMixer.h"
#include "StRoot/StRefMultCorr/StRefMultCorr.h"
#include "StRoot/StEventPlane/StEventPlane.h"
#include "kfEvent.h"

#include <vector>

ClassImp(StPicoMixedEventMaker)

// _________________________________________________________
StPicoMixedEventMaker::StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, StRefMultCorr* grefmultCorrUtil, StEventPlane* eventPlaneMaker,
      char const* outputBaseFileName,  char const* inputHFListHFtree,  char const * kfFileList) :
   StMaker(name), mPicoDst(NULL), mPicoDstMaker(picoMaker),  mPicoEvent(NULL),
   mGRefMultCorrUtil(grefmultCorrUtil), mEventPlane(eventPlaneMaker),
   mOuputFileBaseName(outputBaseFileName), mInputFileName(inputHFListHFtree),
   mEventCounter(0), mTree(NULL), mOutputFileTree(NULL),
   mKfEvent(NULL), mKfFileList(kfFileList), mKfChain(NULL), iiii(0), jjjj(0)
{
   for (int iVz = 0 ; iVz < 10 ; ++iVz)
   {
      for (int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality)
      {
         for (int iEventPlane = 0 ; iEventPlane < 10 ; ++iEventPlane)
         {
            mPicoEventMixer[iVz][iCentrality][iEventPlane] = NULL;
         }
      }
   }

   TH1::AddDirectory(false);

   mOutputFileTree = new TFile(Form("%s.picoMEtree.root", mOuputFileBaseName.Data()), "RECREATE");
   mOutputFileTree->SetCompressionLevel(1);
   mOutputFileTree->cd();
   // -- create OutputTree
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   //if (!mTree)
   //    mTree = new TTree("T", "T", BufSize);
   //mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
   //  mTree->Branch("mixedEvent", "StPicoMixedEvent", &mPicoEventMixer, BufSize, Split);

   // -- constructor
}

// _________________________________________________________
StPicoMixedEventMaker::~StPicoMixedEventMaker()
{
   for (int iVz = 0 ; iVz < 10 ; ++iVz)
   {
      for (int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality)
      {
         for (int iEventPlane = 0 ; iEventPlane < 10 ; ++iEventPlane)
         {
            delete mPicoEventMixer[iVz][iCentrality][iEventPlane];
         }
      }
   }
   //mOutputFileTree->Write();
   mOutputFileTree->Close();
}
// Method should load Q vector stuff from Hao, needs fixing
// _________________________________________________________
bool StPicoMixedEventMaker::loadEventPlaneCorr(StEventPlane const * mEventPlane)
{
   //needs to implement, will currently break maker
   if (!mEventPlane)
   {
      LOG_WARN << "No EventPlane ! Skipping! " << endm;
      return kFALSE;
   }
   if (!mEventPlane->getAcceptEvent())
   {
      // LOG_WARN << "StPicoMixedEvent::THistograms and TProiles NOT found! shoudl check the input Qvector files From HaoQiu ! Skipping this run! " << endm;
      return kFALSE;
   }
   return kTRUE;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Init()
{

   // -------------Next is include KfVertex tree
   // mKfEvent = new kfEvent();
   mKfChain = new TChain("kfEvent");
   std::ifstream listOfKfFiles;
   listOfKfFiles.open(mKfFileList);
   if (listOfKfFiles.is_open())
   {
      std::string kffile;
      while (getline(listOfKfFiles, kffile))
      {
         LOG_INFO << "StPicoD0AnaMaker - Adding kfVertex tree:" << kffile << endm;
         mKfChain->Add(kffile.c_str());
      }
   }
   else
   {
      LOG_ERROR << "StPicoD0AnaMaker - Could not open list of corresponding kfEvent files. ABORT!" << endm;
      return kStErr;
   }
   mKfEvent = new kfEvent(mKfChain);
   // -------------Finish include KfVertex tree----!!!!!!!!

   mOutputFileTree->cd();
   for (int iVz = 0 ; iVz < 10 ; ++iVz)
   {
      for (int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality)
      {
         for (int iEventPlane = 0 ; iEventPlane < 10 ; ++iEventPlane)
         {
            mPicoEventMixer[iVz][iCentrality][iEventPlane] = new StPicoEventMixer(Form("Cent_%i_Vz_%i_EvtPlan_%i", iCentrality, iVz, iEventPlane));
            mPicoEventMixer[iVz][iCentrality][iEventPlane]->setEventBuffer(10);
         }
      }
   }

   // -- reset event to be in a defined state
   //resetEvent();
   mFailedRunnumber = 0;

   return kStOK;
}

// _________________________________________________________
Int_t StPicoMixedEventMaker::Finish()
{
   // -- Inhertited from StMaker
   //    NOT TO BE OVERWRITTEN by daughter class
   //    daughter class should implement FinishHF()
   mOutputFileTree->cd();
   for (int iVz = 0 ; iVz < 10 ; ++iVz)
   {
      for (int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality)
      {
         for (int iEventPlane = 0 ; iEventPlane < 10 ; ++iEventPlane)
         {
            mPicoEventMixer[iVz][iCentrality][iEventPlane]->finish();
            //delete mPicoEventMixer[iVz][iCentrality];
         }
      }
   }
   //mOutputFileTree->Write()

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
   readNextEvent();

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

   if (picoDst->event()->runId() != mKfEvent->mRunId  ||
         picoDst->event()->eventId() != mKfEvent->mEventId)
   {
      LOG_ERROR << " StPicoMixedEventMaker - !!!!!!!!!!!! ATTENTION !!!!!!!!!!!!!" << endm;
      LOG_ERROR << " StPicoMixedEventMaker - SOMETHING TERRIBLE JUST HAPPENED. StPicoDst and KfEvent are not in sync." << endm;
      exit(1);
   }

   // - GRef from Guannan
   if (!mGRefMultCorrUtil)
   {
      LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
      return kStWarn;
   }
   //Lomnitz, need to fix this bs
   StThreeVectorF const pVtx = picoDst->event()->primaryVertex();
   StThreeVectorF const kfVtx(mKfEvent->mKfVx, mKfEvent->mKfVy, mKfEvent->mKfVz);
   if (pVtx.x() != mKfEvent->mVx)               // || pVtx.y() != mKfEvent->mVy || pVtx.z() != mKfEvent->mVz
   {
      LOG_ERROR << " StPicoMixedEventMaker - !!!!!!!!!!!! ATTENTION !!!!!!!!!!!!!" << endm;
      LOG_ERROR << " StPicoMixedEventMaker - SOMETHING TERRIBLE JUST HAPPENED. StPicoDst and KfEvent vertex are not in sync." << endm;
      exit(1);
   }
   // cout<<"pVtx="<<pVtx.x()<<" ; "<<pVtx.y()<<" ;"<<pVtx.z()<<endl;
   // cout<<"kfVtx="<<kfVtx.x()<<" ; "<<kfVtx.y()<<" ;"<<kfVtx.z()<<endl;
   // if(pVtx.x()!=-999) cout<<"pVtx iiii++="<<iiii++<<endl;
   // if(kfVtx.x()!=-999) cout<<"kfVtx jjjj++="<<jjjj++<<endl;

   if (fabs(pVtx.z()) >= 6.0)
      return kStOk;
   mGRefMultCorrUtil->init(picoDst->event()->runId());
   mGRefMultCorrUtil->initEvent(picoDst->event()->grefMult(), pVtx.z(), picoDst->event()->ZDCx()) ;
   int const centrality  = mGRefMultCorrUtil->getCentralityBin9();
   if (centrality < 0 || centrality > 8) return kStOk;
   int const vz_bin = (int)((6 + pVtx.z()) / 1.2) ;
   if (vz_bin < 0  ||  vz_bin > 9) return kStOk;
//     4            55-60%            30-40%
//     5            50-55%            20-30%
//     6            45-50%            10-20%
   //cout<<"Centrality: "<<centrality<<endl;
   if (mFailedRunnumber != picoDst->event()->runId())
   {
      if (!loadEventPlaneCorr(mEventPlane))
      {
         LOG_WARN << "Event plane calculations unavalable! Skipping" << endm;
         mFailedRunnumber = picoDst->event()->runId();
         return kStOK;
      }
   }
   else  return kStOK;

   float const eventPlane = mEventPlane->getEventPlane();
   int const eventPlane_bin = (int)((eventPlane) / 0.3141592) ;
   if (eventPlane_bin < 0  ||  eventPlane_bin > 9) return kStOk;
   //cout << "GUANNAN Xie Check===========" << endl;
   //cout << "mEventPlane->getRunId()=" << mEventPlane->getRunId() << endl;
   //cout << "RunId()=" << picoDst->event()->runId() << endl;
   //cout << "getCentrality()=" << centrality << endl;
   //cout << "mEventPlane->getCentrality()=" << mEventPlane->getCentrality() << endl;
   //cout << "mEventPlane->getEventPlane()=" << mEventPlane->getEventPlane() << endl;
   // cout << "eventPlane_bin=" << eventPlane_bin << endl;

   if (mPicoEventMixer[vz_bin][centrality][eventPlane_bin] -> addPicoEvent(picoDst) ==  true)
      mPicoEventMixer[vz_bin][centrality][eventPlane_bin]->mixEvents();
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
