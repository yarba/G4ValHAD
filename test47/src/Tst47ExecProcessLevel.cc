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
#include "Tst47ExecProcessLevel.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleChange.hh"

#include "TstDiscreteProcessReader.hh"

#include "FTFPWrapper.hh"
#include "QGSPWrapper.hh"
#include "QGSBWrapper.hh"

#include "G4HadronicProcessType.hh"
#include "G4CascadeInterface.hh"
#include "G4BinaryCascade.hh"
#include "G4BinaryLightIonReaction.hh"
#include "G4INCLXXInterface.hh"

#include "G4StateManager.hh"
// #include "G4HadronicDeveloperParameters.hh"
#include "G4FTFTunings.hh"

void Tst47ExecProcessLevel::InitProcess( const TstReader* pset )
{

   G4String name = (pset)->GetPhysics();

   if ( name.find("qgs") != std::string::npos || name.find("ftf") != std::string::npos )
   {
      InitStringModel( name );
      G4cout << " Generator " << name << " initialized" << G4endl; 
      return;
   }

   fProcWrapper = new ProcessWrapper();
   
   G4HadronicInteraction* model = 0;
   
   if ( name.find("bertini") != std::string::npos )
   {
      model = new G4CascadeInterface(); 
   }
   if ( name.find("bertP") != std::string::npos )
   {
      G4CascadeInterface* be = new G4CascadeInterface();
      be->usePreCompoundDeexcitation();
      model = be;
   }
   else if ( name.find("binary") != std::string::npos )
   {
      model = new G4BinaryCascade();
   }
   else if ( name.find("binary_ion") != std::string::npos )
   {
      model = new G4BinaryLightIonReaction();
   }
   else if ( name.find("incl++") != std::string::npos || name.find("inclxx") != std::string::npos )
   {
      model = new G4INCLXXInterface();
   }
   
   //
   // Note: Need to add Elastic ?
   //
   
   if (!model) 
   { 
      G4cout 
	     << " Generator " << name << " is NOT available"
	     << G4endl;
      exit(1);
   } 
    
    
   model->SetMinEnergy(0.);
   model->SetMaxEnergy(15.*GeV);
   
   fProcWrapper->RegisterMe(model);
   
   // I don't have to add it as a dicrete process to the particle's process manager 
   // because it'll be done in another (later) method (beam)
   
   G4cout << " Generator " << name << " initialized" << G4endl; 
   
   return;

}

void Tst47ExecProcessLevel::InitStringModel( const G4String name )
{

   ProcessWrapper* pw = 0;
   
   if ( name.find("ftf") != std::string::npos )
   {

      if ( name.find("_tune") != std::string::npos )
      {
      // example(s) of setting FTF parameters as tunes 
      // (i.e. as groups where selected parameters have been obtained
      //  via collective fits vs thin target data)
      //
      // G4cout << " Is it locked: ? " << G4FTFTunings::Instance()->IsLocked() << std::endl;

      bool isMaster = G4Threading::IsMasterThread();
      G4cout << " Is Master ? " << isMaster << G4endl;
      
      bool isState_PreInit = ( G4StateManager::GetStateManager()->GetCurrentState() == G4State_PreInit );
      G4cout << " Is state PreInit ? " << isState_PreInit << G4endl;

      G4ApplicationState currentstate = G4StateManager::GetStateManager()->GetCurrentState();   
      bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_PreInit);
      G4cout << " Is PreInit set ? " << ok << G4endl;

      isState_PreInit = ( G4StateManager::GetStateManager()->GetCurrentState() == G4State_PreInit );
      
      bool isLocked = ( !isMaster || !isState_PreInit );
      G4cout << " Is locked ?  " << isLocked << G4endl; 
      
// -->      G4FTFTunings::Instance()->SetTuneApplicabilityState( 1,      1 );
// --> below is attempt to set "combined tune" (both baryon-2022 and pion-2022)
      // --> int tuneID = 3;      
      int tuneID = atoi(&(name.back()));  
      G4cout << " tuneID = " << tuneID << G4endl;   
      G4FTFTunings::Instance()->SetTuneApplicabilityState( tuneID,      1 );

// NOTE (JVY): For some reason when trying to set tune via UI
//             the parameter values do not seem to propagate;
//             maybe it should be at a different stage/state ?
//
// -->   G4UImanager* uim = G4UImanager::GetUIpointer();   
// -->   uim->ApplyCommand( "/process/had/models/ftf/selectTuneByIndex 1" );

      ok = G4StateManager::GetStateManager()->SetNewState( currentstate );
      }

      pw = new FTFPWrapper();
   }
   else if ( name.find("qgsp") != std::string::npos )
   {
       pw = new QGSPWrapper();
   }
   else if ( name.find("qgsb") != std::string::npos )
   {
      pw = new QGSBWrapper();
   }

   //
   // Note-1 : Need to add FTFB(inary)
   //
   // Note-2: ftfpU is appears THE SAME as ftfp...
   //
    
   if (!pw) 
   { 
      G4cout 
	     << " generator " << name << " is unavailable"
	     << G4endl;
      exit(1);
   } 
    
   // std::cout << " process = " << fProcWrapper->GetProcessName() << std::endl;
    
   if ( name.find("lund-str-fragm") != std::string::npos )
   {
      pw->UseG4LundStringFragm(true);
   }
    
   pw->Compose();
   
   fProcWrapper = pw;
   
   return;

}

