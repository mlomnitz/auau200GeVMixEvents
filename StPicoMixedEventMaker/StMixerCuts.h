#ifndef StMixerCuts_H
#define StMixerCuts_H
/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Michael Lomnitz (mrlomnitz@lbl.gov)
 *
 * **************************************************
 */
// ---------- QA Switch
//#define __run_w_QA__
//
#include "Rtypes.h"
#include <string>
#include <vector>

#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "TGraph.h"
#include "StMixerCuts.h"

namespace mxeCuts
{
   enum mePid {kKaon, kPion, kProton};
   float const pidMass[3] = { M_KAON_PLUS, M_PION_PLUS, M_PROTON};
   int const nTrig = 5;
   int const mTriggerId[nTrig] = {450050, 450060,
				  450005, 450015,
				  450025 };
   //Event
   //UShort_t const triggerWord = 0x1F; //first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html
   UShort_t const minBiasTrigger = 0x1F;
   float const maxVz = 6.0;// cm.
   float const vzVpdVz = 3.0; // 3 cm.
   float const Verror = 1.0e-5; //
   float const Vrcut = 2.0; //

   //Tracking
   float const minPt = 0.6;//1.2//0.6
   float const nHitsFit = 20;
   float const dca2pVtx = 0.0050;
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

   //Decay Topology Cuts
   int    const decayTopologyCentrality = 7; // minimumCentrality
   double const decayTopologyMinDca = 0.0050;
   double const decayTopologyMaxD0Dca2Vtx = 0.0120;
   //Ultimate1
   //float const dcaV0ToPv[nPtBins] = {0.0062, 0.0047, 0.0040, 0.0041, 0.0042};
   //float const decayLength[nPtBins] = {0.0149, 0.0205, 0.0216, 0.0233, 0.0282};
   //float const cosTheta[nPtBins] = {0.0000, 0.0000, 0.0000, 0.0000, 0.0000};//0.995
   //float const dcaDaughters[nPtBins] = {0.0082, 0.0070, 0.0056, 0.0065, 0.0065}; //0.0050;
   //float const kDca[nPtBins] = {0.0123, 0.0097, 0.0091, 0.0075, 0.0053};//0.008, // minimum
   //float const pDca[nPtBins] = {0.0109, 0.0108, 0.0100, 0.0074, 0.0067};//0.008
   struct TopologicalCuts
   {
     float  RapidityCut;
     int    nPtBins;
     std::vector<float> PtEdge;
     std::vector<float> dcaV0ToPv;
     std::vector<float> decayLength;
     std::vector<float> cosTheta;
     std::vector<float> dcaDaughters;
     std::vector<float> kDca;
     std::vector<float> pDca;
     
   TopologicalCuts(float const RapidityCut,
		   int const nPtBins,
		   std::vector<float> const& PtEdge,
		   std::vector<float> const& dcaV0ToPv,
		   std::vector<float> const& decayLength,
		   std::vector<float> const& cosTheta,
		   std::vector<float> const& dcaDaughters,
		   std::vector<float> const& kDca,
		   std::vector<float> const& pDca):
     RapidityCut(RapidityCut),
       nPtBins(nPtBins),
       PtEdge(PtEdge),
       dcaV0ToPv(dcaV0ToPv),
       decayLength(decayLength),
       cosTheta(cosTheta),
       dcaDaughters(dcaDaughters),
       kDca(kDca),
       pDca(pDca)
     {;};
   };
   TopologicalCuts const D0Cuts_50eff(1., //RapidityCut
			       5, //ptBins
			       {0., 1., 2., 3., 5., 15.}, //ptEdge
			       {0.0044, 0.0036, 0.0031, 0.0026, 0.0032}, //dcaV0ToPv
			       {0.0144, 0.0204, 0.0242, 0.0245, 0.0300}, //decayLength
			       {0.0000, 0.0000, 0.0000, 0.0000, 0.0000}, //cosTheta
			       {0.0069, 0.0048, 0.0044, 0.0049, 0.0047}, //dcaDaughters
			       {0.0119, 0.0110, 0.0109, 0.0106, 0.0080}, //kDca
			       {0.0120, 0.0102, 0.0118, 0.0109, 0.0096}); //pDca
   //
   TopologicalCuts const D0Cuts_150eff(1., //RapidityCut
				     5, //ptBins 
				     {0., 1., 2., 3., 5., 15.}, //ptEdge
				     {0.0072, 0.0053, 0.0047, 0.0042, 0.0062}, //dcaV0ToPv
				     {0.0110, 0.0168, 0.0187, 0.0199, 0.0180}, //decayLength
				     {0.0000, 0.0000, 0.0000, 0.0000, 0.0000}, //cosTheta
				     {0.0077, 0.0078, 0.0074, 0.0068, 0.0066}, //dcaDaughters
				     {0.0105, 0.0068, 0.0080, 0.0066, 0.0041}, //kDca
				     {0.0092, 0.0078, 0.0086, 0.0065, 0.0047}); //pDca
   //
   TopologicalCuts const D0Cuts(1., //RapidityCut
				5, //ptBins 
				{0., 1., 2., 3., 5., 15.}, //ptEdge
				{0.0061, 0.0049, 0.0038, 0.0038, 0.0040}, //dcaV0ToPv
				{0.0145, 0.0181, 0.0212, 0.0247, 0.0259}, //decayLength
				{0.0000, 0.0000, 0.0000, 0.0000, 0.0000}, //cosTheta
				{0.0084, 0.0066, 0.0057, 0.0050, 0.0060}, //dcaDaughters
				{0.0103, 0.0091, 0.0095, 0.0079, 0.0058}, //kDca
				{0.0110, 0.0111, 0.0086, 0.0081, 0.0062}); //pDca 
}
#endif
