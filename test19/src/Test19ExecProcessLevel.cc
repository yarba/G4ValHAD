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

#include "Test19ExecProcessLevel.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleChange.hh"

#include "TstDiscreteProcessReader.hh"
#include "FTFPWrapper.hh"
#include "QGSPWrapper.hh"
#include "G4CascadeInterface.hh"
#include "G4INCLXXInterface.hh"

#include "G4UImanager.hh"
#include "G4StateManager.hh"
// bertini
#include "G4CascadeParameters.hh"
// preco
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"

// ---> #include "G4HadronicDeveloperParameters.hh"

void Test19ExecProcessLevel::InitProcess( const TstReader* pset )
{

   // G4String name = (dynamic_cast<const TstDiscreteProcessReader*>(pset))->GetProcessName();
   G4String name = (pset)->GetPhysics();
      
   ProcessWrapper* pw = 0;
   
   if ( name.find("ftf") != std::string::npos )
   {
      // fProcWrapper = new FTFPWrapper();
      pw = new FTFPWrapper();
   }
   else if ( name.find("qgs") != std::string::npos )
   {
       // fProcWrapper = new QGSPWrapper();
       pw = new QGSPWrapper();
   }
   else if ( name.find("bertini") != std::string::npos )
   {
      //
/*
      G4CascadeParameters::Instance();
      G4UImanager* uim = 0;
      G4ApplicationState currentstate = G4StateManager::GetStateManager()->GetCurrentState();   
      bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_PreInit);
      if ( !ok )
      {
         G4cout << " G4StateManager PROBLEM: can NOT change state to G4State_Idle !" << G4endl;
      } 
      else
      {
         uim = G4UImanager::GetUIpointer();   
         // uim->ApplyCommand( "/process/had/cascade/nuclearRadiusScale 1.5" ); 
         // std::cout << " Updated: Bertini RadiusScale = " << G4CascadeParameters::radiusScale() << std::endl;
         std::cout << " Bertini UsePreCo = " << G4CascadeParameters::usePreCompound() << std::endl;
         uim->ApplyCommand( "/process/had/cascade/usePreCompound 1" ); 
         std::cout << " Update: Bertini UsePreCo = " << G4CascadeParameters::usePreCompound() << std::endl;

         // G4DeexPrecoParameters* precoparams = G4NuclearLevelData::GetInstance()->GetParameters();
	 // precoparams->SetLevelDensity( 0.01/CLHEP::MeV );
	 // std::cout << " Update: PreCo LevelDens = " << precoparams->GetLevelDensity() << std::endl;
	 // precoparams->SetPrecoModelType(1);
	 // precoparams->SetDeexModelType(1);
	 // precoparams->SetUseAngularGen(false);
	 // precoparams->SetDeexChannelsType(fGEM); // it's an enum, with fEvaporation=0, etc.
      }
      ok = G4StateManager::GetStateManager()->SetNewState( currentstate );
*/
      //
      pw = new ProcessWrapper( "BertiniProcessWrapper" );
      G4CascadeInterface* bert = new G4CascadeInterface();
      bert->SetMaxEnergy(15.*GeV);
      pw->RegisterMe(bert);      
   }
   else if ( name.find("incl++") != std::string::npos || name.find("inclxx") != std::string::npos )
   {
      pw = new ProcessWrapper( "INCLXXProcessWrapper" );
      G4INCLXXInterface* inclxx = new G4INCLXXInterface();
      inclxx->SetMinEnergy(1.*MeV); // this is what stands in the INCL-based PL for protons 
                                    // it's 0. for pions, but let's make it uniform
      inclxx->SetMaxEnergy(20.*GeV); // this is what stands in the INCL-based PL for p,n,pi
                                     // overlap is typically set at 15-20GeV with a string model 
				     // (higher energy model)
				     //
				     // in principle INCL++ is good up to 3AGeV for p,n,pi, A(<18)
      pw->RegisterMe(inclxx);
   }
    
   // if (!fProcWrapper) 
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
      // fProcWrapper->UseG4LundStringFragm(true);
      pw->UseG4LundStringFragm(true);
   }
    
   // fProcWrapper->Compose();
   pw->Compose();
   
   fProcWrapper = pw;
   
   return;

}


