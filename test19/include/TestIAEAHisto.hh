#ifndef TestIAEAHisto_h
#define TestIAEAHisto_h

#include "TstHistoSet.hh"

#include <vector>

#include "TH1D.h"

class TestIAEAHisto : public TstHistoSet
{

public:

   TestIAEAHisto( G4String );
   ~TestIAEAHisto();

   void FillEvt( G4VParticleChange*, const G4LorentzVector&, const G4LorentzVector& );

protected:

   virtual void Write( G4int stat=1, G4double scale=1. );

private:

   int                 fNEKinBins;
   double*             fEKinBins;
   
   std::vector<double> fTheta;
   double              fDeltaTheta; 
   std::vector<double> fCosThMin;
   std::vector<double> fCosThMax;
   std::vector<double> fDeltaCosTh;            
   
   std::vector<TH1D*>  fHistoNeutron;
   
   TH1D*               fNSec;

};

#endif
