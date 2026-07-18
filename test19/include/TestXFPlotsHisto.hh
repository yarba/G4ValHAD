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
#ifndef TestXFPlotsHisto_h
#define TestXFPlotsHisto_h

#include "TstHistoSet.hh"

#include <vector>

#include "TH1D.h"

class TestXFPlotsHisto : public TstHistoSet
{

public:

   TestXFPlotsHisto( G4String );
   TestXFPlotsHisto( G4String, double );
   ~TestXFPlotsHisto();

   void FillEvt( G4VParticleChange*, const G4LorentzVector&, const G4LorentzVector& );

protected:

   virtual void Write( G4int stat=1, G4double scale=1. );

private:

   double fSQRTS;
   
   TH1D*  sec_piplus_xf;
   TH1D*  sec_all_xf;
   TH1D*  sec_all_xf_2body;
   TH1D*  sec_all_xf_3body;
   TH1D*  sec_all_xf_manybody;
//   TH1D*  sec_all_xf_5body;
      
   TH1D*  fNSec;

};

#endif
