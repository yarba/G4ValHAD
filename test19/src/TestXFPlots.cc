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
#include "TestXFPlotsHisto.hh"

#include "G4VParticleChange.hh"
#include "G4TrackVector.hh"

#include "G4SystemOfUnits.hh"

TestXFPlotsHisto::TestXFPlotsHisto( G4String htitle )
   : TstHistoSet(htitle)
{

   // basically this is a dummy ctor...
   //
   fSQRTS = 0.;
   sec_piplus_xf = 0;
   sec_all_xf = 0;
   sec_all_xf_2body = 0;
   sec_all_xf_3body = 0;
   sec_all_xf_manybody = 0;
//   sec_all_xf_5body = 0;
   fNSec = 0;

};

TestXFPlotsHisto::TestXFPlotsHisto( G4String htitle, double sqrts )
   : TstHistoSet(htitle)
{

   fSQRTS = sqrts;
   
   std::string title;
   std::string outcome; 

   outcome = " -> X + pi+";
   title = htitle + outcome;   
   sec_piplus_xf = new TH1D( "piplus_xf", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

   outcome = " -> X";
   title = htitle + outcome;
   sec_all_xf = new TH1D( "all_xf", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

   outcome = " -> 2-body";
   title = htitle + outcome;
   sec_all_xf_2body = new TH1D( "all_xf_2Body", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

   outcome = " -> 3-body";
   title = htitle + outcome;
   sec_all_xf_3body = new TH1D( "all_xf_3Body", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

   outcome = " -> 4-or-more-body";
   title = htitle + outcome;
   sec_all_xf_manybody = new TH1D( "all_xf_4orMoreBody", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

//   outcome = " -> 5-body";
//   title = htitle + outcome;
//   sec_all_xf_5body = new TH1D( "all_xf_5body", title.c_str(), 39, -0.975, 0.975 ); // 200, -1., 1. );

   fNSec = new TH1D( "NSec", htitle.c_str(), 50, 0., 50. );
   
}

TestXFPlotsHisto::~TestXFPlotsHisto()
{

   if ( sec_piplus_xf ) delete sec_piplus_xf;
   if ( sec_all_xf ) delete sec_all_xf;
   if ( sec_all_xf_2body ) delete sec_all_xf_2body;
   if ( sec_all_xf_3body ) delete sec_all_xf_3body;
   if ( sec_all_xf_manybody ) delete sec_all_xf_manybody;
//   if ( sec_all_xf_5body ) delete sec_all_xf_5body;
   if ( fNSec ) delete fNSec;

};

void TestXFPlotsHisto::FillEvt( G4VParticleChange* aChange, const G4LorentzVector&, const G4LorentzVector& labp ) 
{

   if ( fSQRTS <= 0. || !fNSec ) return;
   
   int NSec = aChange->GetNumberOfSecondaries();
   
   if ( NSec <= 0 ) return;
   
   fNSec->Fill( (double)NSec );

   const G4DynamicParticle* sec = 0;
   
   G4ThreeVector boostLabp = labp.boostVector();

   for ( int is=0; is<NSec; ++is )
   {
   
      sec = aChange->GetSecondary(is)->GetDynamicParticle();
      const G4String& pname = sec->GetDefinition()->GetParticleName();
      
      G4ThreeVector mom = sec->GetMomentum() / GeV ;
      double mass = sec->GetDefinition()->GetPDGMass() / GeV;
      double ekin = sec->GetKineticEnergy() / GeV ;
	
      G4LorentzVector boostSec( mom, ekin+mass );
      boostSec.boost(-boostLabp);
	
      double xF  = 2 * (boostSec.z()) / (fSQRTS/GeV);
      
      if ( pname == "pi+" )
      {
         sec_piplus_xf->Fill( xF );
      }
      
      sec_all_xf->Fill( xF );
      
      if ( NSec == 2 )
      {
         sec_all_xf_2body->Fill( xF );
      }
      else if ( NSec == 3 )
      {
         sec_all_xf_3body->Fill( xF );
      }
      else if ( NSec > 3 )
      {
         sec_all_xf_manybody->Fill( xF );
      }
//      else if ( NSec == 5 )
//      {
//         sec_all_xf_5body->Fill( xF );
//      }
      
   }   
   
   return;

}

void TestXFPlotsHisto::Write( G4int stat, G4double /* xsec */ )
{

   if ( !fNSec ) return;
   
   fNSec->Scale( 1./((double)stat), "width" );
   fNSec->Write();
   
   sec_piplus_xf->Scale( 1./((double)stat), "width" );
   sec_piplus_xf->Write();
   
   sec_all_xf->Scale( 1./((double)stat), "width" );
   sec_all_xf->Write();
   
   sec_all_xf_2body->Scale( 1./((double)stat), "width" );
   sec_all_xf_2body->Write();

   sec_all_xf_3body->Scale( 1./((double)stat), "width" );
   sec_all_xf_3body->Write();

   sec_all_xf_manybody->Scale( 1./((double)stat), "width" );
   sec_all_xf_manybody->Write();

//   sec_all_xf_5body->Scale( 1./((double)stat), "width" );
//   sec_all_xf_5body->Write();

   return;

}
