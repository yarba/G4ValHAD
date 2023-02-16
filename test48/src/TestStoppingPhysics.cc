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
//
// -------------------------------------------------------------
//
//      History: based on object model of
//      ---------- TestStoppingPhysics -------
//                   by Julia Yarba 
// 
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "TestStoppingPhysics.hh"

#include "G4VRestProcess.hh"

#include "G4PhysicsTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "G4MuonMinus.hh"
#include "G4PionMinus.hh"
#include "G4KaonMinus.hh"
#include "G4AntiProton.hh"
#include "G4AntiNeutron.hh"
#include "G4SigmaMinus.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"

// essential since 10.6.r03 due to some "cleanups"
//
#include "G4ShortLivedConstructor.hh"

#include "G4SystemOfUnits.hh"

#include "G4HadronicAbsorptionBertini.hh"
#include "G4HadronicAbsorptionFritiof.hh"

//
// new development, in place since g4.9.6.b01
//
#include "G4MuonMinusCapture.hh"

#include "G4PreCompoundModel.hh"

// Bertini cascade
// local code for pi-, K- & Sigma-
// (very original implementation before the standard interface
//  went in place; still kept here for cross-checks)
//
// NOTE: as of 10.7-series, is it still needed ?
//
// #include "TestBertiniStopping.hh"
 
#if defined (USE_MUCAPTURE)
// new Muon code
#include "G4MuonMinusCapturePhysics.hh"
#endif

// --> ??? --> #include "G4SpecialMuMinusCapturePrecompound.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

TestStoppingPhysics::TestStoppingPhysics(G4int verbose):
#if defined (USE_MUCAPTURE)
  theMuonMinusCaptureConstructor(new G4MuonMinusCapturePhysics(verbose)),
#endif
  theProcess(0), theProcessMan(0), verboseLevel(verbose)
{

  //G4DecayPhysics pDecayPhysics;
  //pDecayPhysics.ConstructParticle(); // it is an example; we do not do it here though
  
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();  
  
  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
  
  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();
  
  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();
  
   G4ShortLivedConstructor pShortLivedConstructor;
   pShortLivedConstructor.ConstructParticle();  

//   will be done in the main
//
//   G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
//   partTable->SetReadiness();

#if defined (USE_MUCAPTURE)
  // for the new mu capture
  theMuonMinusCaptureConstructor->ConstructParticle();
#endif
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

TestStoppingPhysics::~TestStoppingPhysics()
{
  //delete theDeExcitation;
  //delete thePreCompound;
  if ( theProcess )                     delete theProcess;
  if ( theProcessMan)                   delete theProcessMan;
#if defined (USE_MUCAPTURE)
  if ( theMuonMinusCaptureConstructor ) delete theMuonMinusCaptureConstructor;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VProcess* TestStoppingPhysics::GetProcess(const G4String& gen_name,
		                            const G4String& part_name)
{

  G4cout << part_name << " "
         << gen_name 
         << " initializing 0"
         << G4endl;         

  if(theProcess) delete theProcess;
  theProcess = 0;
  if ( theProcessMan) delete theProcessMan;
  theProcessMan = 0;
    
  if (part_name == "anti_proton")   
  {
     theProcessMan = new G4ProcessManager(G4AntiProton::AntiProton());
  }
  else if (part_name == "anti_neutron") 
  {
     theProcessMan = new G4ProcessManager(G4AntiNeutron::AntiNeutron());
  }
  else if (part_name == "pi-") 
  {
     theProcessMan = new G4ProcessManager(G4PionMinus::PionMinus());
  }
  else if (part_name == "kaon-")  
  {
     theProcessMan = new G4ProcessManager(G4KaonMinus::KaonMinus());
  }
  else if (part_name == "mu-")  
  {

#if defined (USE_MUCAPTURE)

    // in case of new mucapture muons we need more particles, processes and Process Managers
    // creating process managers if not existing
    G4ParticleTable*   partTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = partTable->GetIterator();

    auto myParticleIterator=GetParticleIterator();
    myParticleIterator->reset();

    while( (*myParticleIterator)() ) {
      G4ParticleDefinition* particle = myParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (verboseLevel>1){
        G4cout <<"TestStoppingPhysics::GetProcess"
               <<" : Checking Process Manager for "
               << particle->GetParticleName() 
               << G4endl;
      }

      if ( pmanager == 0) {
        // no process manager
        if (verboseLevel>0){
          G4cout <<"TestStoppingPhysics::GetProcess"
                 <<" : Creating Process Manager for "
                 << particle->GetParticleName() 
                 << G4endl;
        }
        pmanager = new G4ProcessManager(particle); // FIXME leak?
                                                   // something needs
                                                   // to clean it up
        particle->SetProcessManager(pmanager);
      }
    }

    theMuonMinusCaptureConstructor->ConstructProcess();
    theProcessMan = G4MuonMinus::MuonMinus()->GetProcessManager();

#else

    theProcessMan = new G4ProcessManager(G4MuonMinus::MuonMinus());
	 
    if (verboseLevel>1){   // use verboseLevel to suppress compilation warning
	    G4cout <<"TestStoppingPhysics::GetProcess() not using new mucapture"<<G4endl;
    }		
#endif

  }
  else if ( part_name == "sigma-" )
  {
     theProcessMan = new G4ProcessManager(G4SigmaMinus::SigmaMinus());
  }

  if(!theProcessMan) return 0;

  G4cout << part_name << " "
         << gen_name 
         << " initializing 1"
         << G4endl;         


  // Choose generator
  //
  if(gen_name == "stopping" || gen_name == "captureUpdate"
#if defined (USE_MUCAPTURE)
     || gen_name == "newmustopping" 
#endif
     ) 
  {

    if (part_name == "anti_neutron" )
    {
       theProcess = new G4HadronicAbsorptionFritiof();
    }
    else if ( part_name == "mu-")
    {

      if (gen_name == "stopping" )
      {
          theProcess = new G4MuonMinusCapture();
      }
      else if ( gen_name == "captureUpdate" )
      {
          theProcess = new G4MuonMinusCapture();
      }
#if defined (USE_MUCAPTURE) 
      else if (gen_name == "newmustopping" )
        {
          //          theMuonMinusCaptureConstructor->ConstructProcess();
          // constructed earlier in the case of mu- (FIXME should have been guarded by an if ???)
          theProcess = theMuonMinusCaptureConstructor->GetMuonMinusAtomicCaptureProcess();
          // new MuonStopping is done differently and we need to exit without "re adding" the process
          G4cout <<  " Model <"
                 << gen_name << "> is initialized"
                 << G4endl;         
          return theProcess;
        }
#endif
    }
  } 
  else if ( gen_name == "Bertini" || gen_name == "BertiniPreCo" )
  {

     if ( part_name == "kaon-" ) 
     {
	theProcess = new G4HadronicAbsorptionBertini();
     }
     else if ( part_name == "sigma-" )
     {
	theProcess = new G4HadronicAbsorptionBertini();	
     }
     else if ( part_name == "pi-" )
     {
	theProcess = new G4HadronicAbsorptionBertini();
     }

  }
  else if ( gen_name == "FTF")
  {
     if ( part_name == "anti_proton" )
     {
        theProcess = new G4HadronicAbsorptionFritiof();
     }
  }
  else 
  {
    G4cout << gen_name
           << " generator is unkown - no hadron production" << G4endl;
  }
  
  theProcessMan->AddRestProcess(theProcess);

  G4cout <<  " Model <"
         << gen_name << "> is initialized"
         << G4endl;
  
  return theProcess;

}	

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
