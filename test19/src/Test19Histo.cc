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

#include "TSystem.h"

#include "Test19Histo.hh"
#include "TstReader.hh"

#include "TestNA49Histo.hh"
#include "TestNA61ProtonBeamHisto.hh"
#include "TestNA61PiplusBeamHisto.hh"
#include "TestHARPHisto.hh"
#include "TestSASM6EHisto.hh"
#include "TestIAEAHisto.hh"
//
# include "TestXFPlotsHisto.hh"

#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <iomanip>


Test19Histo::Test19Histo( const TstReader* pset )
   : TstHisto(pset)
{

   TH1::SetDefaultSumw2();

   if ( pset->GetExpDataSet() == "NA61" )
   {
      if (pset->GetBeamParticle() == "proton")
      { 
         std::cout << "Initialize NA61 histo for proton beam" << std::endl;
	 fHistoSet = new TestNA61ProtonBeamHisto( fHistoTitle );
      }
      else if ( pset->GetBeamParticle() == "pi+" )
      {
         std::cout << "Initialize NA61 histo for pi+ beam" << std::endl;
         fHistoSet = new TestNA61PiplusBeamHisto( fHistoTitle );         
      }
      else
      {
         std::cout << "NA61 BENCHMARK : UNKNOWN BEAM TYPE "  
	           << pset->GetBeamParticle() << std::endl;
         exit(1);
      }
      fHistoDirName  = "na61-histo";
   }
   else if ( pset->GetExpDataSet() == "NA49" )
   {
      fHistoSet = new TestNA49Histo( fHistoTitle );
      // fHistoSet = new Tst23NA49Histo( fHistoTitle );
      fHistoDirName  = "na49-histo";
   }
   else if ( pset->GetExpDataSet() == "HARP" )
   {
      // fHistoSet = new TestHARPHisto( fHistoTitle );
      fHistoSet = new TestHARPHisto( pset );
      fHistoDirName = "harp-histo";
   }
   else if ( pset->GetExpDataSet() == "SASM6E" )
   {
      fHistoSet = new TestSASM6EHisto( fHistoTitle );
      fHistoDirName = "sasm6e-histo";
   }
   else if ( pset->GetExpDataSet() == "IAEA" )
   {
      fHistoSet = new TestIAEAHisto( fHistoTitle );
      fHistoDirName = "iaea-histo";
   } 
   else if ( pset->GetExpDataSet() == "xFplots" )
   {
      pset->SyncKinematics();
      double sqrts = 0.;
      G4LorentzVector tmp( 0.0, 0.0, 0.0, 0.0 );
/*
      G4string matname = pset->GetTargetMaterial();
      G4Material* mat = 0;
      G4Element* ele = 0;
      if ( matname.find("G4") == std::string::npos ) 
      {
         matname = "G4_" + matname;
         mat = G4NistManager::Instance()->FindOrBuildMaterial(fMatName);
         if (!mat) 
         {
            G4cout << "Material <" << matname << "> is not found" << G4endl;
            exit(1);
         }
      }
      ele = mat->GetElement(0);
*/      
      double tgt_mass = 938.272 * CLHEP::MeV; // proton mass
      G4LorentzVector tgt_4mom = tmp;
      tgt_4mom.setE( tgt_mass );
      
      G4LorentzVector proj_4mom = tmp;
      proj_4mom.setVect( pset->GetBeamMomentum()*pset->GetDirection() );
      proj_4mom.setE( pset->GetBeamEnergy() );

      G4LorentzVector psum;
      psum = tmp;
      psum = proj_4mom + tgt_4mom;
      double s2 = psum.mag2();
      sqrts = std::sqrt( s2 ); 
            
      fHistoSet = new TestXFPlotsHisto( fHistoTitle, sqrts );
      fHistoDirName = "xfplots-histo";
   }
   
     
   // fHistoSet->SetDoResDecay(pset->ForseResDecay());
   fHistoSet->SetDoResDecay(fDoResDecay);   

}

TFile* Test19Histo::OpenHistoFile()
{

   // AccessPathName actually returns o (false) is directory ** exists **
   // and 1 (true) if it does NOT
   //
   if ( gSystem->AccessPathName( fHistoDirName.c_str() ) )
   {   
      gSystem->mkdir( fHistoDirName.c_str() );
   }
   
   G4String tmp_bname;
   if ( fBeam == "pi+" )
   {
      tmp_bname = "piplus";
   }
   else if ( fBeam == "pi-" )
   {
      tmp_bname = "piminus";
   }
   else if ( fBeam == "kaon+" )
   {
      tmp_bname = "kplus";
   }
   else if ( fBeam == "kaon-" )
   {
      tmp_bname = "kminus";
   }
   else
   {
      tmp_bname = fBeam;
   }

//    G4String fname = fHistoDirName + "/" + fBeam + fTarget + fBeamMomentum + fModel;
   // --> G4String fname = fHistoDirName + "/" + tmp_bname + fTarget + fBeamMomentum + fModel;
   G4String fname = tmp_bname + fTarget + fBeamMomentum + fModel;
   
   // check if such file name already exists in fHistoDi which is 
   // possible when running with different configs of the same model
   // i.e. different "Universe" ("Univ")
   //
   // but first check if the model name already contains "Univ";
   // if so, do nothing;
   // otherwise extend the filename with "Univ" + serial number
   //
   // NOTE: but only do so if non-defult model config which is
   //       known via fModelConfig!=NULL
   //
   // NOTE/FIXME: there might be more elegant ideas/approaches 
   // but for now we'll go with this straightforwad approach
   //
   
   if ( fModelConfig )
   {
      if ( fModel.find("Univ") == std::string::npos )
      {
   
         TSystemDirectory sdir( "mydir",fHistoDirName.c_str() );
         TList* sfiles = sdir.GetListOfFiles();
         if (sfiles) 
         {
            TSystemFile* sfile;
	    TIter next(sfiles);
	    int counter = 0;
	    while ( ( sfile=(TSystemFile*)next() ) )
	    {
	       G4String sfname = sfile->GetName();
	       if ( sfname == "." || sfname == ".." ) continue;
	       G4cout << " On disk: " << sfname << G4endl;
	       if ( sfname.find(fname) != std::string::npos )
	       {
	          counter++;
	       }
	    }
	    G4cout << " counter = " << counter << G4endl;
	    if ( counter >= 1 ) fname += ( "Univ" + std::to_string(counter) );	 
         }
      
      }
   }
   
   fname = fHistoDirName + "/" + fname;
   
   if ( fJobID > -1 )
   {
      fname += "-" + std::to_string( fJobID );
   }  
   
   if ( fDoResDecay )
   {
      fname += "-with-decays";
   }
   fname += ".root";

   return new TFile( fname.c_str(), "recreate" );

}
