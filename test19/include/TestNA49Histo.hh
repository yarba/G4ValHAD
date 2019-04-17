//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#ifndef TestNA49Histo_h
#define TestNA49Histo_h

#include "TstHistoSet.hh"

#include <vector>
#include <map>

#include "TProfile.h"

class G4VParticleChange;

class TestNA49Histo : public TstHistoSet  
{

public:

   TestNA49Histo( G4String );
   ~TestNA49Histo();
   
   void FillEvt( G4VParticleChange*, const G4LorentzVector&, const G4LorentzVector& );

protected:

   virtual void Write( G4int stat=1, G4double scale=1. );

private:
   
   double CalculateBinWeight( const G4LorentzVector&, const G4String&, double, double, double, int, double );

   TH1D*              fHistoNSec;

   int     fNPBinsXF;
   double* fPBinsXF;
   int     fNPBinsPT;
   double* fPBinsPT;
   
   std::vector<TH1D*> fHistoSecProton;
   std::vector<TH1D*> fHistoPTProton;
    
   int      fNPbarBinsXF;
   double*  fPbarBinsXF;
   int*     fNPbarBinsPT;
   double** fPbarBinsPT;
   
   std::vector<TH1D*> fHistoSecAntiProton;
   std::vector<TH1D*> fHistoPTAntiProton;

   int                fNPiBinsXF;
   double*            fPiBinsXF;
   int                fNPiBinsPT;
   double*            fPiBinsPT;

   std::vector<TH1D*> fHistoSecPiMinus; 
   std::vector<TH1D*> fHistoSecPiPlus; 
   std::vector<TH1D*> fHistoSecNeutron;   
   std::vector<TH1D*> fHistoPTPiMinus;
   std::vector<TH1D*> fHistoPTPiPlus;
   
   G4VParticleChange* fInteraction;

};

#endif
