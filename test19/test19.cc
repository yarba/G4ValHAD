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

#include "G4Timer.hh"

// random engine/seed settings
#include "Randomize.hh"

// #include "Test19Reader.hh"
#include "TstDiscreteProcessReader.hh"

#include "Test19ExecProcessLevel.hh"

#include "G4VParticleChange.hh"

#include "Test19Histo.hh"

#include "G4SystemOfUnits.hh"

#include "G4ParticleTable.hh"
#include "G4HadronicProcessStore.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4BGGPionInelasticXS.hh"

using namespace std;

int main(int argc, char** argv) 
{

   TstDiscreteProcessReader* theConfigReader = new TstDiscreteProcessReader();
   
   // Control on input
   if (argc < 2) 
   {
      G4cout << "Input file is not specified! Exit" << G4endl;
      exit(1);
   }
  
   theConfigReader->OpenAppConfig( argv[1] );
   theConfigReader->Help();
  
   do 
   {

      theConfigReader->ProcessConfig();
      if ( theConfigReader->IsDone() ) break; // double protection because 
                                              // in the previous cycle it stopped at #run
           
      Test19ExecProcessLevel* exec = new Test19ExecProcessLevel( theConfigReader );
      
      // exec->Init( theConfigReader ); // leftover from old design, will delete later

      TstHisto* histo = 0;
      histo = new Test19Histo( theConfigReader );
      if ( !histo )
      {
         G4cout << " Histograms NOT booked" << G4endl;
         exit(1);
      }
      
      G4VParticleChange* aChange = 0;
      
      G4int NEvts = theConfigReader->GetNEvents();
      
//      std::cout << " Engine is " << CLHEP::HepRandom::distributionName() << std::endl;
//      CLHEP::HepRandom::showEngineStatus(); 
      
      
      G4Timer timer;
      timer.Start();
      
      auto start = std::chrono::high_resolution_clock::now();
      
      for (G4int iter=0; iter<NEvts; ++iter) 
      {


/*         if ( iter%10000 == 0 )
	 {
	    G4cout << " event # " << iter << G4endl;
	 }
*/	 
	 // CLHEP::HepRandom::showEngineStatus(); 

         aChange = exec->DoEvent();

	 // aChange->DumpInfo();
	 	 	 
	 histo->FillEvt( aChange, exec->GetBeam()->GetLabV(), exec->GetBeam()->GetLabP() );
	 
         G4int nsec = aChange->GetNumberOfSecondaries();
         for (G4int i=0; i<nsec; ++i) 
         {   
/*
            G4Track* tmpsec = aChange->GetSecondary(i);
	    std::cout << " tmpsec type = " << tmpsec->GetDefinition()->GetParticleName() << std::endl;
	    std::cout << " ekin = " << tmpsec->GetKineticEnergy() << std::endl;
	    std::cout << " parent = " << tmpsec->GetParentID() << std::endl;
*/	    
	    delete aChange->GetSecondary(i);
         } 
         aChange->Clear();

      } // end loop over events
      
      timer.Stop();
      G4cout << " CPU = " << timer.GetUserElapsed() << G4endl;
      G4cout << " Real Time = " << timer.GetRealElapsed() << G4endl;      
      
/* In principle, this scenario works just fine 
   meaning that there's no need for XSecOnTarget thing
   BUT !!! Somehow it crashes with 11.2.p01 on extracting the XS 
   while working just fine with 11.2.r06 (debug or prof) 
   NOTE: OK, in this particular case the issue was in NOT having
   cs->BuildPhysicsTable(*partDef) executedc prior to trying 
   to extract the XS.
   Still not quite clear why it was working without in case of 11.2.r06...
   */
   
      G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
      G4ParticleDefinition* partDef = partTable->FindParticle( exec->GetBeam()->GetBeamPartName() );
      G4DynamicParticle dParticle( partDef, exec->GetBeam()->GetBeamMomentum() );
      dParticle.DumpInfo();
      G4Material* mat = exec->GetTarget()->GetCurrentMaterial();
      const G4Element* elm = mat->GetElement(0);

      std::cout << mat << std::endl;
      std::cout << elm << std::endl;

  G4VCrossSectionDataSet* cs = 0;
  if ( partDef == G4Proton::Definition() || partDef == G4Neutron::Definition() ) 
  {
      cs = new G4BGGNucleonInelasticXS( partDef );
  } 
  else if ( partDef == G4PionPlus::Definition() || partDef == G4PionMinus::Definition() ) 
  {
      cs = new G4BGGPionInelasticXS( partDef );
  }
  if (cs)
  { 
      cs->BuildPhysicsTable(*partDef);
      double scale_test = cs->GetCrossSection( &dParticle, elm );
      std::cout << " scale_test = " << scale_test << std::endl;
  }
/* */
      std::cout << " XSecOnTarget = " << exec->GetXSecOnTarget() << std::endl;
      std::cout << " XSecOnTarget/mb = " << exec->GetXSecOnTarget()/millibarn << std::endl;

      G4HadronicProcessStore* const store = G4HadronicProcessStore::Instance();
      
      // store->PrintInfo(partDef);
      store->Dump(1);
      double xsec_from_store = store->GetInelasticCrossSectionPerAtom(partDef, 
                                                                      dParticle.GetKineticEnergy(), 
								      elm ); // , fMaterial);
      
      histo->Write( theConfigReader->GetNEvents(), exec->GetXSecOnTarget()/millibarn );
      
      if ( histo ) delete histo; // delete histograms after writing them out
      
  const auto stop = std::chrono::high_resolution_clock::now();
  const auto diff = stop - start;
  const auto time = static_cast<G4double>(
      std::chrono::duration_cast<std::chrono::microseconds>(diff).count()) / 1e6;
  G4cout << G4endl;
  G4cout << "Processed " << NEvts << " events (collisions) in " 
         << std::scientific << time << " seconds."
         << " Average: " << std::defaultfloat << (time * 1E3 / NEvts) << " ms / event." 
         << G4endl;
  G4cout << G4endl;

      delete exec;

   } while(!theConfigReader->IsDone()); // end loop over "runs"

   G4cout << "###### End of test #####" << G4endl;
  
   delete theConfigReader; 
   
   return 0;

}
