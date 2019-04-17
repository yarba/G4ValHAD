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

#include "QGSBWrapper.hh"

#include "G4TheoFSGenerator.hh"

#include "G4QGSMFragmentation.hh"
#include "G4LundStringFragmentation.hh"

#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4PreCompoundModel.hh"
#include "G4BinaryCascade.hh"

#include "G4SystemOfUnits.hh"
#include "G4HadronicParameters.hh"

QGSBWrapper::QGSBWrapper( const G4String& name, G4ProcessType type )
   : ProcessWrapper(name,type)
{
}

void QGSBWrapper::Compose()
{

   G4TheoFSGenerator* gen = new G4TheoFSGenerator();

   fStringModel      = new G4QGSModel< G4QGSParticipants >; // specific
   
   fCascade          = new G4BinaryCascade(); // common
   
   fStringDecay      = new G4ExcitedStringDecay( new G4QGSMFragmentation() ); 
   fStringModel->SetFragmentationModel( fStringDecay ); // common interface but different input

   gen->SetQuasiElasticChannel( new G4QuasiElasticChannel() ); // specific
   gen->SetTransport( fCascade );
   gen->SetHighEnergyGenerator( fStringModel );
      
   gen->SetMinEnergy(GeV);  // common
   gen->SetMaxEnergy( G4HadronicParameters::Instance()->GetMaxEnergy() ); 
   
   RegisterMe( gen );  
   
   return; 

}
