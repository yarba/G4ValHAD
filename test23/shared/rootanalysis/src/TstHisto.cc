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

#include "TstHisto.hh"
#include "ModelConfig.hh"

#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>

TstHisto::TstHisto( const TstReader* pset )
    : fJobID(pset->GetJobID()), 
      fBeam(pset->GetBeamParticle()), 
      fTarget(pset->GetTargetMaterial()), 
      fModel(pset->GetPhysics()),
      fHistoSet(0), 
      fHistoFile(0),
      fDoResDecay(pset->ForceResDecay()),
      fModelConfig(0)
{
   
   char ene[4];
   sprintf( ene, "%3.1f", pset->GetBeamMomentum()/GeV );
   fBeamMomentum = "";
   fBeamMomentum.append( ene );
   fBeamMomentum += "GeV";
   
   fHistoTitle = fBeam + " + " + fTarget;
      
   if ( pset->GetPhysicsConfig().find("default") == std::string::npos ) // non-default model params
   {      

      if ( !fModelConfig) fModelConfig = new ModelConfig(false); // non-default physics
      const std::vector< std::pair<std::string,double> >* params; 
      if ( pset->IsPhysicsList() )
      {
         std::cout << " FOR A PHYSICS LIST, INSERT CONFIG(S) FOR ALL MODELS" << std::endl;
         std::vector<std::string> conf_models = pset->GetConfiguredModels();
	 for ( size_t i=0; i<conf_models.size(); ++i )
	 {
	    params = pset->GetConfig(conf_models[i]);
            for ( size_t i1=0; i1<(*params).size(); ++i1 )
            {
               fModelConfig->Insert( pset->GetPhysics(), 
	                             ((*params)[i1]).first, 
				     ((*params)[i1]).second );
	    }
	 }
      }
      else
      {
	 // here we need to find the matching model given that 
	 // the model name in the master config can be e.g.,
	 // bertini-LvgOff or ftfp_tune3, etc.
	 //
	 std::vector<std::string> conf_models = pset->GetConfiguredModels();
	 std::string mod = boost::algorithm::to_lower_copy( pset->GetPhysics() );
	 // use int; can't use size_t because it's *unsigned* 
	 int imatch = -1;
	 for ( size_t ii=0; ii<conf_models.size(); ++ii )
	 {
	    if ( mod.find(conf_models[ii]) != std::string::npos ||
	         conf_models[ii].find(mod) != std::string::npos )
	    {
	       imatch = ii;
	       break;
	    }
	 }
	 if ( imatch > -1 && imatch < conf_models.size() )
	 {	 
	    std::cout << " FOR " << pset->GetPhysics() << 
	                 " FOUND MATCHING MODEL: " << conf_models[imatch] << std::endl;
	    params = pset->GetConfig( conf_models[imatch] /* mod */ );
            for ( size_t i1=0; i1<(*params).size(); ++i1 )
            {
//              std::cout << " Inserting parameter: " << ((*params)[i1]).first 
//	                << " , value: " << ((*params)[i1]).second << std::endl;
               fModelConfig->Insert( conf_models[imatch], /* pset->GetPhysics(), */ 
	                             ((*params)[i1]).first, 
				     ((*params)[i1]).second );
            }
	 }
      }
   }
   
}

TstHisto::~TstHisto()
{

   if ( fHistoSet )  delete fHistoSet;
   if ( fHistoFile ) delete fHistoFile;
   if ( fModelConfig ) delete fModelConfig;

}

void TstHisto::Write( G4int stat, G4double scale )
{

   if ( fHistoFile ) delete fHistoFile;

   fHistoFile = OpenHistoFile();
   
   if ( fModelConfig ) fModelConfig->Print();

   fHistoFile->cd();
   
   // NOTE: here I need to find the *directory*
   //       (and it's possible to treat a file as a dir !!!)
   //       rather that writing into a file !!!
   //       in other words, the following won't work... OR WILL IT ???
   //       ACTUALLY IT DOES WORK !!!
   //       however, if handling a ROOT file in an interactive session
   //       must explicitly include the following:
   // #include "../test23/shared/rootanalysis/include/ModelConfig.hh"
   //
   if ( fModelConfig ) fHistoFile->WriteObject( fModelConfig, fModelConfig->GetName().c_str() );
   //
   //       another possible working example is the following:
   // std::string dirname = "bertini";
   // TDirectory* mydir = fHistoFile->mkdir(dirname.c_str());
   // mydir->cd();
   // mydir->WriteObject( fModelConfig, "modelconfig" );
   // ... or if inherit from TObject and has Write(...) (see how to inherit from TObject !) 
   // once one cd() to a dir, one can do as follows:
   //
   // fModelConfig->Write( "modelconfig" )
   
   // WriteHisto( stat );
   fHistoSet->Write( stat, scale );

   fHistoFile->Close();
   
   return;

}

TFile* TstHisto::OpenHistoFile()
{

   G4String tmp_bname;
   
   if ( fBeam == "pi+" )
   {
      tmp_bname = "piplus";
   }
   else if ( fBeam == "pi-" )
   {
      tmp_bname = "piminus" ;
   }
   else
   {
      tmp_bname = fBeam;
   }
   
   
//   G4String fname = fBeam + fTarget + fBeamMomentum + fModel;
   G4String fname = tmp_bname + fTarget + fBeamMomentum + fModel;
   
   if ( fJobID > -1 )
   {
      fname += "-" + std::to_string( fJobID );
   }  
   fname += ".root";

   return new TFile( fname.c_str(), "recreate" ); 

}
