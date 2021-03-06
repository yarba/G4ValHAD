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

#include "ExecBase.hh"

#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

#include "G4ParticleChange.hh"
#include "G4DynamicParticle.hh"

#include "G4GenericIon.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"
//
// essential since 10.6.r03 due to some "cleanups"
//
#include "G4ShortLivedConstructor.hh"

#include "G4ProcessManager.hh"

#include "G4Gamma.hh"

#include "G4StateManager.hh"
#include "G4ForceCondition.hh"

#include "TstReader.hh"

#include "Randomize.hh"

ExecBase::~ExecBase()
{

/* original code that would be needed if rndm engine is created explicitly (via new) 
   if ( fRndmEngine ) delete fRndmEngine;
   fRndmEngine=0; 
*/

}

void ExecBase::Init( const TstReader* config )
{
   
   // Choose the Random engine 
   //
  fRndmEngine = G4Random::getTheEngine();

/* original code that is kept here as an example/option
   fRndmEngine = new CLHEP::RanecuEngine;
   // -> fRndmEngine = new CLHEP::HepJamesRandom; // default engine in CLHEP 2.3.x series  
   CLHEP::HepRandom::setTheEngine( fRndmEngine );
// --> something old ? -->   G4Random::setTheEngine( fRndmEngine );   
*/

   // technically speaking, one should ensure that the seed is in the correct range:
   // long seed = onfig->GetRndmSeed() % 900000000
   //
   CLHEP::HepRandom::setTheSeed( config->GetRndmSeed() ); // just something... to be on the safe side
      
   if(!G4StateManager::GetStateManager()->SetNewState(G4State_PreInit))
       G4cout << "G4StateManager PROBLEM! " << G4endl;

   InitParticles();   
   
   if(!G4StateManager::GetStateManager()->SetNewState(G4State_Idle))
      G4cout << "G4StateManager PROBLEM! " << G4endl;

   
   std::cout << " Starting with seed = " << config->GetRndmSeed() << std::endl;
   // std::cout << " Engine name is " << fRndmEngine->engineName() << std::endl;
   fRndmEngine->showStatus();
   
   return;

}

void ExecBase::InitParticles()
{

   // physics needs to be initialized before the 1st use of particle table,
   // because it constructs particles - otherwise the table is just empty
   //
   // in principle, this code is similar to the G4DecayPhysics
   // but it's better to do it explicitly in order to see what it takes
   //
   G4MesonConstructor pMesonConstructor;
   pMesonConstructor.ConstructParticle();

   G4BaryonConstructor pBaryonConstructor;
   pBaryonConstructor.ConstructParticle();  
  
   // This is needed because starting 9.6.ref05 G4IonTable::CtreateIon(...)
   // explicitly checks if generic ion has a process manager
   //
   G4GenericIon* gion = G4GenericIon::GenericIon();
   gion->SetProcessManager(new G4ProcessManager(gion));    
   //
   G4IonConstructor pIonConstructor;
   pIonConstructor.ConstructParticle();
  
   G4LeptonConstructor pLeptonConstructor;
   pLeptonConstructor.ConstructParticle();
  
   G4BosonConstructor pBosonConstructor;
   pBosonConstructor.ConstructParticle();

   G4ShortLivedConstructor pShortLivedConstructor;
   pShortLivedConstructor.ConstructParticle();  

   G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
   partTable->SetReadiness();

   // NOT really needed, at least so far
   //
   // --> G4IonTable* ionTable = partTable->GetIonTable();
   // --> ionTable->CreateAllIon();
   // --> ionTable->CreateAllIsomer();

  return;
}
