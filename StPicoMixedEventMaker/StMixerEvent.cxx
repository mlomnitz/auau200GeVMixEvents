#include "StMixerEvent.h"
#include "TVector2.h"
#include <limits>
StMixerEvent::StMixerEvent() :  mVtx(StThreeVectorF()),
   mBField(std::numeric_limits<float>::quiet_NaN())
{
}
StMixerEvent::StMixerEvent(StMixerEvent *t) : mVtx(t->mVtx), mBField(t->mBField),
   mWeight(t->mWeight), mTracks(t->mTracks),
   mEventKaons(t->mEventKaons), mEventPions(t->mEventPions), mQ(t->mQ)
{
}
StMixerEvent::StMixerEvent(StThreeVectorF vtx, float b, TVector2 Q, float weight) :
   mWeight(weight), mQ(Q), mVtx(vtx), mBField(b)
{
}
void StMixerEvent::addTrack(StMixerTrack t)
{
   mTracks.push_back(t);
   return;
}
void StMixerEvent::addPion(int arrayId)
{
   mEventPions.push_back(arrayId);
   return;
}
void StMixerEvent::addKaon(int arrayId)
{
   mEventKaons.push_back(arrayId);
   return;
}
