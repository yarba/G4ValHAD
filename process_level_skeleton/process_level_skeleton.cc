
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

#include "globals.hh"
#include "G4PhysicalConstants.hh"

#include "G4ios.hh"

#include "G4Material.hh"
#include "G4IsotopeVector.hh"
#include "G4ElementVector.hh"
#include "G4NistManager.hh"

#include "G4ProcessManager.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleChange.hh"

#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

#include "G4ParticleChange.hh"
#include "G4DynamicParticle.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4GenericIon.hh"

#include "G4Gamma.hh"

#include "G4ForceCondition.hh"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Step.hh"
#include "G4GRSVolume.hh"
#include "G4GRSSolid.hh"
#include "G4Region.hh"

#include "G4TrackingManager.hh"

#include "G4StateManager.hh"
// #include "G4Navigator.hh"
#include "G4Timer.hh"

// random engine/seed settings
#include "Randomize.hh"

#include "G4SystemOfUnits.hh"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

// Bertini cascade itself !!!
#include "G4CascadeInterface.hh"
#include "G4CascadeParameters.hh"
#include "G4PhotoNuclearProcess.hh" // gamma-N hadronic process
#include "G4HadronInelasticProcess.hh" // generic hadron inelastic process
#include "G4HadronInelasticDataSet.hh"

// #include "ProcessWrapper.hh"
// FTF(P) business
#include "FTFPWrapper.hh"
#include "G4HadronicDeveloperParameters.hh" 

#include "G4UImanager.hh"

void construct_particles();
// void pseudo_geom( G4Region* );
void pseudo_geom();
void track_and_step( const std::string&, double, double );

G4Region* fRegion = 0;
G4Track*  fTrack  = 0;
G4Step*   fStep   = 0;

using namespace std;

// --> int main(int argc, char** argv) {
int main() {

  G4cout <<"========================================================" <<G4endl;
  G4cout <<"======      Gamma-Nuclear Test Start       ========" <<G4endl;
  G4cout <<"========================================================" <<G4endl;

  G4String  namePart = "proton";
  G4String  nameMatRaw  = "C";

/*
  G4String  nameGen  = "Bertini";
  G4double  m_p      = 3824.*MeV;
*/

  G4String  nameGen  = "FTFP";
  G4double  m_p      = 5000.*MeV;

  G4double theStep   = 0.01*micrometer; 
  G4Material* material = 0;

  G4int     nevt     = 100000;
  
  // Choose and setup the Random engine, if desired
  // (this is optional; if not specified, Gent4 will run on its default)
  //
  // G4long myseed   = 135799753;
  // G4long myseed   = 12345;
  //
  // NOTE (JVY): this seed below, together with FTF settings 
  //             make the job crash after had012 exception !!!
  //
  G4long myseed = 1635;
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(myseed);

/* ---> BERTINI CONFIGURATION INTERFACE

   // If you plan to use Bertini cascade, and you wish to turn ON its internal verbosity, you can do it as follows.
   // You have to bear in mind a couple of rules:
   // 1. Geant4 will need to be in a certain "state", i.e. "Idle" - or UI command(s) are NOT guarantee to propagate.
   // 2. You have to "ping" G4CascadeParameters before any changes via UI make in (can't remember why, something technical in the guts).
   // 3. Also bear in mind that all UI cards have to go in BEFORE Bertini is instantiated.
   // 

   G4CascadeParameters::Instance();
   // bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_Idle);
   bool ok = G4StateManager::GetStateManager()->SetNewState(G4State_PreInit);
   if ( !ok )
   {
      G4cout << " G4StateManager PROBLEM: can NOT change state to G4State_Idle !" << G4endl;
   } 
   else
   {
      G4UImanager* uim = G4UImanager::GetUIpointer();   
      // uim->ApplyCommand( "/process/had/cascade/verbose 2" ); // NOTE: 1 will NOT work - any output from Bertini start with verb>1...
                                                             //       I think you can also use 3 or 4...
							     //       Default=0
      // Test switches
      //
      // WARNING !!!
      // This command BREAKS the defaults because any attempt to use it will set the NUCRAD2PAR to a NON-default !!!
      // See lines 188-189 in G4CascadeParamMessenger.cc - need to be fixed there !!!
      //
      // uim->ApplyCommand( "/process/had/cascade/useTwoParamNuclearRadius 0" );
      //
      // uim->ApplyCommand( "/process/had/cascade/usePreCompound 1" );

      // Test numeric parameters
      //
      // uim->ApplyCommand( "/process/had/cascade/nuclearRadiusScale 1." );
      
   }
//   G4cout << "Bertini verbosity: new setting = " << G4CascadeParameters::verbose() << G4endl;
  
   // Now we need to be in the PreInit state to create particles
   //
   if(!G4StateManager::GetStateManager()->SetNewState(G4State_PreInit))
      G4cout << "G4StateManager PROBLEM! " << G4endl;
*/

/* ---> FTF(P) CONFIGURATION INTERFACE */

   G4HadronicDeveloperParameters& HDP = G4HadronicDeveloperParameters::GetInstance(); 
   HDP.Set( "FTF_NUCDESTR_P1_TGT", 0.00723018 ); 
   HDP.Set( "FTF_NUCDESTR_P1_ADEP_TGT", true ); 
   HDP.Set( "FTF_NUCDESTR_P2_TGT", 15.3415 );
   HDP.Set( "FTF_NUCDESTR_P3_TGT", 1.62041 ); 
/* */



   // physics needs to be initialized before the 1st use of particle table,
   // because it constructs particles - otherwise the table is just empty
   //
   construct_particles(); 

   // PSEUDO-Geometry
   //
   // NOTE: it does not even need to have a valid ponter to material in the log.vol.
   //
   pseudo_geom(); // it'll set global pointer fRegion
  
    // NOTE: original (default) name of material is Cu
    //       one can create material(s) using traditional G4 machinery
    //       as an alternative one can use G4Nist DB (see below)
    //       but in such case material's name should start with G4_
    //       one can as as show below, although there're other options ^_^
    //       (osMat is defined at the very beginning of the code)
    
    ostringstream osMat(ios_base::out|ios_base::app);// to enable appending in output operations
    osMat.clear();
    osMat.str("G4_");
    osMat << nameMatRaw;
    G4String nameMat = osMat.str();

    G4cout << "###### Material: " << nameMat << " derived from " << nameMatRaw << G4endl;

    material = G4NistManager::Instance()->FindOrBuildMaterial(nameMat);

    if (!material) {
      G4cout << "Material <" << nameMat << "> is not found" << G4endl;
      exit(1);
    }

    G4ParticleDefinition* part = 
      (G4ParticleTable::GetParticleTable())->FindParticle(namePart);

    // Define hadronic process and outfit it with an engine (had.model)
    //
    G4HadronicProcess* proc = 0;
    G4HadronicInteraction* model = 0;
    ProcessWrapper* pw = 0;
    if ( nameGen == "Bertini" )
    {
       if ( namePart == "gamma" )
       {
          proc = new G4PhotoNuclearProcess();
       } 
       else
       {
          // proc = new G4HadronInelasticProcess( "hadronInelastic", part );
          // proc = new G4HadronicProcess( "hadronInelastic", fHadronic );
          // proc->AddDataSet( new G4HadronInelasticDataSet() ); // NOTE: this is what G4HadronicInelasticProcess does
          proc = new ProcessWrapper( "BertiniProcessWrapper" );
       }
       model = new G4CascadeInterface();
       // make sure the process & model pointers are valid
       //
       assert(proc);
       assert(model);
       // configure the model to a specific energy range
       //
       // Here we use momentum although the validity range is defined in terms of energy
       //
       // model->SetMinEnergy(0.9*m_p);
       // model->SetMaxEnergy(1.1*m_p);
       model->SetMinEnergy(0.0);
       model->SetMaxEnergy(10.*GeV);
       //       
       // now assign model to the process
       //
       proc->RegisterMe(model);
    }
    else if ( nameGen == "FTFP" )
    {
       pw = new FTFPWrapper();
       pw->Compose();
       proc = pw;
    }   
           
    if (!proc) {
      G4cout << "For particle: " << part->GetParticleName()
             << " generator " << nameGen << " is unavailable"
             << G4endl;
      exit(1);
    } 
           
    // let process managed of the hadronic process that we've just instatiated
    //
/*
    G4ProcessManager* man = 0;
    man = new G4ProcessManager( part );
    man->AddDiscreteProcess(proc);
*/        
    G4double mass = part->GetPDGMass();
    G4double energy = sqrt(m_p*m_p + mass*mass);

    // if for any reason you need target properties, below is an example of how to extract some...
    //
    // const G4Element* elm = material->GetElement(0);
    // G4int A = (G4int)(elm->GetN()+0.5);
    // G4int Z = (G4int)(elm->GetZ()+0.5);
    //
    // amass would be needed e.g. to calculate Lorentz boost, etc. (e.g. via G4LorentzVector)
    // 
    // G4double amass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(Z, A);
    
    track_and_step( namePart, energy, theStep ); // it'll set global pointers fTrack and fStep
        
    
    // NOTE: materials is a local pointer, so we set it here
    //       ... although we can make it global...
    //
    fStep->GetPreStepPoint()->SetMaterial( material );
    fStep->GetPostStepPoint()->SetMaterial( material );

    if(!G4StateManager::GetStateManager()->SetNewState(G4State_Idle))
      G4cout << "G4StateManager PROBLEM! " << G4endl;

    G4Timer* timer = new G4Timer();
    timer->Start();

    G4VParticleChange* aChange = 0;

    for (G4int iter=0; iter<nevt; ++iter) 
    {
    
//       if ( iter > 28420 )
//       {
          if ( (iter%1000) == 0 )
          {
             std::cout << " event # " << iter << std::endl;
             // ---> CLHEP::HepRandom::showEngineStatus(); 
          }
//       }
       
       
       G4double e0 = energy-mass;
       fTrack->SetKineticEnergy(e0); // it'll set ekin to the underlying dynamic particle   
       // ---> std::cout << " Track EKin = " << fTrack->GetKineticEnergy() << std::endl;
       
       
       // model a single interaction
       //
       aChange = proc->PostStepDoIt(*fTrack,*fStep);
       
       // ---> aChange->DumpInfo();
              
       // get number of secondaries from each interaction
       //
       G4int nsec = aChange->GetNumberOfSecondaries();
       
/* example looping over secondaries 
       for (G4int i=0; i<nsec; i++) {
       
            const G4Track*           ssTrack = aChange->GetSecondary(i);
            G4TrackStatus ssts =  	ssTrack->GetTrackStatus();
            const G4DynamicParticle* ssdp = ssTrack->GetDynamicParticle();
            const G4ParticleDefinition* ssdpd = ssdp->GetDefinition();
            G4String ssName = ssdpd->GetParticleName();
            G4cout << "Secondary: " << i
                   << " is " << ssName
                   << ", " << ssdp->GetPDGcode()
                   << ", status: " << ssts
                   << " with kinetic energy of " 
                   << ssdp->GetKineticEnergy()/GeV 
                   << " and momentum of " 
                   << ssdp->GetTotalMomentum()/GeV << " GeV"
                   << G4endl;
       }
*/       
       
       // memory cleanup after this interaction (and before the next one)
       // NOTE: included with ProcessWrapper
/*
       for (G4int i=0; i<nsec; i++) 
       {   
          delete aChange->GetSecondary(i);
       } 
       aChange->Clear();
*/       
    } // end loop over events
    
    timer->Stop();
    G4cout << "  " << *timer << G4endl;
    delete timer;
    
    //delete process
    //
    delete proc;
    
    // NOTE: it's not excluded that I've even forgotted to delete a couple of objects...
    //
    delete fTrack;
    delete fStep;
    delete fRegion;

  G4cout << "###### End of test #####" << G4endl;
  
  return 0;

}

void construct_particles()
{

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
  
   G4ParticleTable::GetParticleTable()->SetReadiness();

   return;

}

// void pseudo_geom( G4Region* rFrame )
void pseudo_geom()
{

   // NOTE: it does not even need to have a valid ponter to material in the log.vol.
   //
  G4PVPlacement* pFrame = new G4PVPlacement( 0, G4ThreeVector(), "Box",
                                             new G4LogicalVolume( new G4Box("Box", 100.0*cm, 100.0*cm, 100.0*cm), 
					                          0, "Box", 0, 0, 0
					                        ),
					     0, false, 0);
  
  assert(pFrame);

  fRegion = new G4Region("Region"); // needed by tracking manager
  pFrame->GetLogicalVolume()->SetRegion( fRegion );
  fRegion->AddRootLogicalVolume( pFrame->GetLogicalVolume() );
  
/* NOTE: if I try to set material pointer here, back in main it's still NULL...
    // NOTE: original (default) name of material is C
    //       one can create material(s) using traditional G4 machinery
    //       as an alternative one can use G4Nist DB (see below)
    //       but in such case material's name should start with G4_
    //       one can as as show below, although there're other options ^_^
    //       (osMat is defined at the very beginning of the code)
    
    ostringstream osMat(ios_base::out|ios_base::app);// to enable appending in output operations
    osMat.clear();
    osMat.str("G4_");
    osMat << nameMatRaw;
    G4String nameMat = osMat.str();

    G4cout << "###### Material: " << nameMat << " derived from " << nameMatRaw << G4endl;

    material = G4NistManager::Instance()->FindOrBuildMaterial(nameMat);

    if (!material) {
      G4cout << "Material <" << nameMat << "> is not found" << G4endl;
      exit(1);
    }
*/
   return;

}

void track_and_step( const std::string& namePart, double energy, double theStep )
{

   G4ThreeVector aPosition  = G4ThreeVector(0.,0.,0.);
   G4double      aTime      = 0. ;
   G4ThreeVector aDirection = G4ThreeVector(0.0,0.0,1.0);
   aDirection = aDirection.unit();
   //
   // NOTE: It's essential to make create the Dynamic particle object
   // because G4Track ctor does NOT instantiate, it only sets the pointer !!! 
   //
   fTrack = new G4Track( new G4DynamicParticle( (G4ParticleTable::GetParticleTable())->FindParticle(namePart),
                                                aDirection, energy ),
                         aTime,
			 aPosition );

   //
   // one needs to "connect" the track with the (pseudo)geometry
   //
   G4TouchableHandle fpTouchable(new G4TouchableHistory());
   fTrack->SetTouchableHandle(fpTouchable);
        
   // -------- Step

   fStep = new G4Step();
   fStep->SetTrack(fTrack);
   fTrack->SetStep(fStep);

   fStep->SetPreStepPoint( new G4StepPoint() );
   fStep->GetPreStepPoint()->SetPosition( aPosition );
   // gStep->GetPreStepPoint()->SetMaterial( material );
   fStep->GetPreStepPoint()->SetSafety( 10000.*CLHEP::cm );
   
   G4ThreeVector bPosition = aDirection * theStep;
   bPosition += aPosition;
   fStep->SetPostStepPoint( new G4StepPoint() );
   fStep->GetPostStepPoint()->SetPosition( bPosition );
   // gStep->GetPostStepPoint()->SetMaterial( material );
   fStep->GetPostStepPoint()->SetSafety( 10000.*CLHEP::cm );
   //
   fStep->SetStepLength( theStep );

   return;

}
