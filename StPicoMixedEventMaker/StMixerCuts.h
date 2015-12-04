#ifndef StMixerCuts_H
#define StMixerCuts_H
/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Michael Lomnitz (mrlomnitz@lbl.gov)
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>

#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "TGraph.h"
#include "StMixerCuts.h"

namespace mxeCuts
{
   enum mePid {kKaon, kPion, kProton};
   float const pidMass[3] = { M_KAON_PLUS, M_PION_PLUS, M_PROTON};

   //Event
   UShort_t const triggerWord = 0x1F; //first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html
   float const maxVz = 6.0;// cm.
   float const vzVpdVz = 3.0; // 3 cm.
   float const Verror = 1.0e-5; //
   float const Vrcut = 2.0; //

   //Tracking
   float const minPt = 0.6;//1.2//0.6
   float const nHitsFit = 20;
   float const dca2pVtx = 0.008;
   bool const mRequireHft = true;
   float const Eta = 1.0;
   //PID
   // pions
   float const nSigmaPion = 3.0;
   float const tofOneOverBetaDiffPion = 0.03;
   // kaons
   float const nSigmaKaon = 2.0;
   float const tofOneOverBetaDiffKaon = 0.03;

   float const massMin = 0.;
   float const massMax = 2.5;
   int const massBins = 250;
   //Topology QA
   float const QAmassMin = 1.828;
   float const QAmassMax = 1.892;

   int   const nPtBins = 5;
   float const PtEdge[nPtBins + 1] = {0., 1., 2., 3., 5., 15.};
   //Ultimate1
   //float const dcaV0ToPv[nPtBins] = {0.0062, 0.0047, 0.0040, 0.0041, 0.0042};
   //float const decayLength[nPtBins] = {0.0149, 0.0205, 0.0216, 0.0233, 0.0282};
   //float const cosTheta[nPtBins] = {0.0000, 0.0000, 0.0000, 0.0000, 0.0000};//0.995
   //float const dcaDaughters[nPtBins] = {0.0082, 0.0070, 0.0056, 0.0065, 0.0065}; //0.0050;
   //float const kDca[nPtBins] = {0.0123, 0.0097, 0.0091, 0.0075, 0.0053};//0.008, // minimum
   //float const pDca[nPtBins] = {0.0109, 0.0108, 0.0100, 0.0074, 0.0067};//0.008
   float const RapidityCut = 1.0;
   //50% Efficiency of Ultimate 2
   float const dcaV0ToPv[nPtBins] = {0.0044, 0.0036, 0.0031, 0.0026, 0.0032};
   float const decayLength[nPtBins] = {0.0144, 0.0204, 0.0242, 0.0245, 0.0300};
   float const cosTheta[nPtBins] = {0.0000, 0.0000, 0.0000, 0.0000, 0.0000};//0.995
   float const dcaDaughters[nPtBins] = {0.0069, 0.0048, 0.0044, 0.0049, 0.0047}; //0.0050;
   float const kDca[nPtBins] = {0.0119, 0.0110, 0.0109, 0.0106, 0.0080};//0.008, // minimum
   float const pDca[nPtBins] = {0.0120, 0.0102, 0.0118, 0.0109, 0.0096};//0.008

}
#endif
