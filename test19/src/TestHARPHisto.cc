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
#include "TFile.h"

#include "TestHARPHisto.hh"

#include "G4VParticleChange.hh"
#include "G4TrackVector.hh"

#include "G4SystemOfUnits.hh"

#include <fstream>

TestHARPHisto::TestHARPHisto( G4String ht )
   : TstHistoSet(ht),
     fNThetaBinsFW(4), fThetaMinFW(0.05), fDeltaThetaFW(0.05),
     fNThetaBinsLA(9), fThetaMinLA(0.35), fDeltaThetaLA(0.2),
     fNTheta1(14), fThetaMin1(0.15), fDeltaTheta1(0.2)
{

   BookStandardHisto(ht);

}

TestHARPHisto::TestHARPHisto( const TstReader* pset )
   : TstHistoSet(pset),
     fNThetaBinsFW(4), fThetaMinFW(0.05), fDeltaThetaFW(0.05),
     fNThetaBinsLA(9), fThetaMinLA(0.35), fDeltaThetaLA(0.2),
     fNTheta1(14), fThetaMin1(0.15), fDeltaTheta1(0.2)
{
   
   G4String ht = pset->GetBeamParticle() + " + " + pset->GetTargetMaterial();
   
   BookStandardHisto(ht);
   if ( fIncludeExpData ) 
   {
      fDataSubDir = "/test23/harp-exp-data";
      BookHistoWithExpData(pset);
   }
   
}

TestHARPHisto::~TestHARPHisto()
{

   if ( fHistoNSec ) delete fHistoNSec;
   
   for (size_t i=0; i<fHistoSecPiMinusFW.size(); i++ )
   {
      delete fHistoSecPiMinusFW[i];
   }
   for (size_t i=0; i<fHistoSecPiPlusFW.size(); i++ )
   {
      delete fHistoSecPiPlusFW[i];
   }
   for (size_t i=0; i<fHistoSecProtonFW.size(); i++ )
   {
      delete fHistoSecProtonFW[i];
   }

   for (size_t i=0; i<fHistoSecPiMinusLA.size(); i++ )
   {
      delete fHistoSecPiMinusLA[i];
   }
   for (size_t i=0; i<fHistoSecPiPlusLA.size(); i++ )
   {
      delete fHistoSecPiPlusLA[i];
   }
   for (size_t i=0; i<fHistoSecProtonLA.size(); i++ )
   {
      delete fHistoSecProtonLA[i];
   }

//------------------

   for ( size_t i=0; i<fHistoSecPiMinusMomBins.size(); ++i )
   {
      delete fHistoSecPiMinusMomBins[i];
   }
   for ( size_t i=0; i<fHistoSecPiPlusMomBins.size(); ++i )
   {
      delete fHistoSecPiPlusMomBins[i];
   }
   
   delete [] fMomBinsLA;
   
//------------------

   for ( size_t i=0; i<fExpPiMinusFW.size(); ++i )
   {
      delete fExpPiMinusFW[i];
   }
   fExpPiMinusFW.clear();   

   for ( size_t i=0; i<fSimPiMinusFW.size(); ++i )
   {
      delete fSimPiMinusFW[i];
   }
   fSimPiMinusFW.clear();   

   for ( size_t i=0; i<fExpPiPlusFW.size(); ++i )
   {
      delete fExpPiPlusFW[i];
   }
   fExpPiPlusFW.clear();   
   
   for ( size_t i=0; i<fSimPiPlusFW.size(); ++i )
   {
      delete fSimPiPlusFW[i];
   }
   fSimPiPlusFW.clear();   

   for ( size_t i=0; i<fExpProtonFW.size(); ++i )
   {
      delete fExpProtonFW[i];
   }
   fExpProtonFW.clear();   

   for ( size_t i=0; i<fSimProtonFW.size(); ++i )
   {
      delete fSimProtonFW[i];
   }
   fSimProtonFW.clear();   

}

void TestHARPHisto::BookStandardHisto( G4String ht )
{

  G4String htitle;
  G4String hname;
  
   fHistoNSec = new TH1D( "NSec", ht.c_str(), 100, 0., 100. );

//   std::ostringstream osTitle1(std::ios_base::out|std::ios_base::app);
//   std::ostringstream osTitle2(std::ios_base::out|std::ios_base::app);
//   std::ostringstream osTitle3(std::ios_base::out|std::ios_base::app);
       
   G4double thetaMin = 0.;
   G4double thetaMax = 0.;
   std::string theta_bin_fw;
   std::string theta_bin_la;

   double parbins_fw[] = { 0.5, 1.0, 1.5, 2., 2.5, 3., 3.5, 4., 5., 6.5, 8. };
   int    nparbins_fw = sizeof(parbins_fw) / sizeof(double) - 1;
      
   for ( G4int i=0; i<fNThetaBinsFW; i++ )
   {
      thetaMin = fThetaMinFW + fDeltaThetaFW*i;
      thetaMax = thetaMin + fDeltaThetaFW;
      
      std::ostringstream osTitle1;
      std::ostringstream osTitle2;
      std::ostringstream osTitle3;
      
      osTitle1.clear();
      osTitle1 << thetaMin;
      theta_bin_fw = osTitle1.str() + " < theta < ";
      osTitle2.clear();
      osTitle2 << thetaMax;
      theta_bin_fw += osTitle2.str();
      theta_bin_fw += "(rad)";
   
      osTitle3.clear();
      osTitle3 << i;
      
      htitle = ht + " -> X + pi-, " + theta_bin_fw;  
      hname = "piminus_FW_" + osTitle3.str();         
//      fHistoSecPiMinusFW.push_back( new TH1D( hname.c_str(), htitle.c_str(), 80, 0., 8.0 ) );
      fHistoSecPiMinusFW.push_back( new TH1D( hname.c_str(), htitle.c_str(), nparbins_fw, parbins_fw ) );

      htitle = ht + " -> X + pi+, " + theta_bin_fw;
      hname = "piplus_FW_" + osTitle3.str();
//      fHistoSecPiPlusFW.push_back( new TH1D( hname.c_str(), htitle.c_str(), 80, 0., 8.0 ) );
      fHistoSecPiPlusFW.push_back( new TH1D( hname.c_str(), htitle.c_str(), nparbins_fw, parbins_fw ) );

      htitle = ht + " -> X + p, " + theta_bin_fw; 
      hname = "proton_FW_" + osTitle3.str();
      fHistoSecProtonFW.push_back( new TH1D( hname.c_str(), htitle.c_str(),nparbins_fw, parbins_fw ) ); 

   }
   
   
   double parbins_la[] = { 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8 };
   int    nparbins_la = sizeof(parbins_la) / sizeof(double) - 1;
   
   for ( G4int i=0; i<fNThetaBinsLA; i++ )
   {
     
      thetaMin = fThetaMinLA + fDeltaThetaLA*i;
      thetaMax = thetaMin + fDeltaThetaLA; 
     
      std::ostringstream osTitle1;
      std::ostringstream osTitle2;
      std::ostringstream osTitle3;

      osTitle1.clear();
      osTitle1 << thetaMin;
      theta_bin_la = osTitle1.str() + " < theta < ";
      osTitle2.clear();
      osTitle2 << thetaMax;
      theta_bin_la += osTitle2.str();
      theta_bin_la += "(rad)";
      
      osTitle3.clear();
      osTitle3 << i;
      
      htitle = ht + " -> X + pi-, " + theta_bin_la;  
      hname = "piminus_LA_" + osTitle3.str();         
//      fHistoSecPiMinusLA.push_back( new TH1D( hname.c_str(), htitle.c_str(), 50, 0., 2.5 ) );
      fHistoSecPiMinusLA.push_back( new TH1D( hname.c_str(), htitle.c_str(), nparbins_la, parbins_la ) );
//      hname = "pimimus_mom_total_" + osTitle3.str();
//      fHistoSecPiMinusTot.push_back( new TH1D( hname.c_str(), htitle.c_str(), 50, 0., 2.5) );	

      htitle = ht + " -> X + pi+, " + theta_bin_la;
      hname = "piplus_LA_" + osTitle3.str();
//      fHistoSecPiPlusLA.push_back( new TH1D( hname.c_str(), htitle.c_str(), 50, 0., 2.5 ) );
      fHistoSecPiPlusLA.push_back( new TH1D( hname.c_str(), htitle.c_str(), nparbins_la, parbins_la ) );
//      hname = "piplus_mom_total_" + osTitle3.str();
//      fHistoSecPiPlusTot.push_back( new TH1D( hname.c_str(), htitle.c_str(), 50, 0., 2.5 ) );	

      htitle = ht + " -> X + p, " + theta_bin_la;
      hname = "proton_LA_" + osTitle3.str();
      fHistoSecProtonLA.push_back( new TH1D( hname.c_str(), htitle.c_str(), nparbins_la, parbins_la ) );

   }  

//---------------
//
// Extras for Mu2e- originally inspired by the G4 review in Aug.2015
//

   double* thetabins = new double[fNTheta1+1];
   for ( int ith=0; ith<=fNTheta1; ++ith )
   {
      thetabins[ith] = fThetaMin1 + fDeltaTheta1*ith;
   }
   
   std::string momtitle = "";
   fNMomBinsLA = nparbins_la+1;
   fMomBinsLA  = new G4double[fNMomBinsLA+1];
   
   fMomBinsLA[0] = 0.05;
   for ( int ip=0; ip<nparbins_la; ++ip )
   {
      fMomBinsLA[ip+1] = parbins_la[ip];
   }
   fMomBinsLA[fNMomBinsLA] = parbins_la[nparbins_la];
   
   for (int ip=0; ip<fNMomBinsLA; ++ip )
   {
      
      std::ostringstream osTitle1;
      std::ostringstream osTitle2;
      std::ostringstream osTitle3;
      
      osTitle1.clear();
      osTitle1 << fMomBinsLA[ip];
      momtitle = osTitle1.str() + " < P(pi-), GeV/c < ";
      osTitle2.clear();
      osTitle2 << fMomBinsLA[ip+1];
      momtitle += osTitle2.str();
      
      osTitle3.clear();
      osTitle3 << ip;
      
      //htitle = ht + " -> X + pi-, " + momtitle;  
      htitle = momtitle;
      hname = "piminus_mom_" + osTitle3.str();
      fHistoSecPiMinusMomBins.push_back( new TH1D( hname.c_str(), htitle.c_str(), fNTheta1, thetabins ) );
      
      momtitle = osTitle1.str() + " < P(pi+), GeV/c < ";
      momtitle += osTitle2.str();
      
      //htitle = ht + " -> X + pi+, " + momtitle;
      htitle = momtitle;
      hname = "piplus_mom_" + osTitle3.str();
      fHistoSecPiPlusMomBins.push_back( new TH1D( hname.c_str(), htitle.c_str(), fNTheta1, thetabins ) );
      
   }
   
   delete [] thetabins;

   return;

}

void TestHARPHisto::BookHistoWithExpData( const TstReader* pset )
{
   
   if ( !fIncludeExpData ) return;
   
   G4cout << " REQUSTED INCLUDE of HARP EXP.DATA (fIncludeExpData=" << fIncludeExpData << ")" << G4endl;
   
   // for now, check for env.var. G4INSTALL or G4ValHAD
   // and if defined look under fDataSubDir for data files
   // NOTE/FIXME: potentially, one can supply direct path...
   
   
   G4String g4install = G4String( gSystem->Getenv("G4INSTALL") );
   G4String g4valhad  = G4String( gSystem->Getenv("G4ValHAD") );
   G4String datadir = "";
   
   if ( g4install.empty() && g4valhad.empty() )
   {
      // one last try from the workdir
      G4cout << " INCLUDE EXP.DATA REQUSTED BUT NEITHER G4INSTALL NOR G4ValHaD DEFINED" << G4endl;
      G4cout << " TRY ../" << fDataSubDir << G4endl;
      datadir = "../" + fDataSubDir;
      if ( gSystem->AccessPathName( datadir.c_str() ) ) // 1(true) is returned if NOT found
      {
         datadir = "";            
      }
   }
   else
   {
      G4cout << " G4INSTALL=" << g4install << G4endl;
      G4cout << " GValHAD=" << g4valhad << G4endl;
   }
   
   if ( datadir.empty() )
   {
      if ( !g4install.empty() ) // choose G4INSTALL in first place
      {
         datadir = g4install; 
      }
      else // otherwise if G4INSTALL is NOT setup, take G4ValHAD
      {
      datadir = g4valhad; 
      } 
   
      // now check if the directory contains or doesn't contain 
      // "tests/ctests_integration" subdir (part of refXX dev.releases)
      //
   
      G4String testpath = datadir + "/tests/ctests_integration";
      if ( gSystem->AccessPathName( testpath.c_str() ) ) // returns 1(true) if *does NOT* exist
      {
         G4cout << " " << testpath << " does NOT exist; do not expand the path" << G4endl;
         datadir += ( "/" + fDataSubDir + "/" + pset->GetTargetMaterial() + "/" );
      }
      else  // returns 0(false) if *does* exist
      {
         G4cout << " " << testpath << " does exist;expand the path" << G4endl;
         datadir = testpath + "/" + fDataSubDir + "/" + pset->GetTargetMaterial() + "/";
      }
   }
   else
   {
      datadir += ( "/" + pset->GetTargetMaterial() + "/" );
   }
      
   G4String ht = pset->GetBeamParticle() + " + " + pset->GetTargetMaterial();

   char ene[4];
   sprintf( ene, "%3.1f", pset->GetBeamMomentum()/GeV );
   G4String bmom = "";
   bmom.append( ene );
   bmom += "GeV";

   G4String sec[3] = { "piminus", "piplus", "proton" };
   
   const int NSetsFW = 4;
   const int NSetsLA = 9;

   float** AngleBinHARP = 0;
   int*    NPointsHARP = 0; 
   float** XMinHARP = 0;
   float** XMaxHARP = 0;
   float** YHARP = 0;
   float** EYHARP = 0;
   //
   // NOTE: based on the info from the ref. below, errors on the HARP data are published
   //       as a total of stat&sys (except 2% overal normalization uncertanty which is not included)
   //       see here:
   //       http://lss.fnal.gov/archive/thesis/2000/fermilab-thesis-2008-26.pdf
   //       Journal publications are not very explicit on this matter
   //       all in all, we haven't been using it all although maybe we should revisit the matter...
   //
   float   NORM_UNCERTANTY = 0.02;

   // FW datasets exist for all 3 tyles of secondaries
   // NOTE/FIXME: some datasets for sec.proton are incomplete; need to revisit/reinstate:
   //             e.g. for Cu, Pb, and/or Ta targets
   //             also, for C target the 9GeV sec.proton dataset jammed and needs correction !
   
   AngleBinHARP = new float*[2];
   AngleBinHARP[0] = new float[NSetsFW];
   AngleBinHARP[1] = new float[NSetsFW]; 

   NPointsHARP = new int[NSetsFW];

   XMinHARP = new float*[NSetsFW];
   XMaxHARP = new float*[NSetsFW];
   YHARP    = new float*[NSetsFW];
   EYHARP   = new float*[NSetsFW]; 

   for ( int is=0; is<3; ++is )
   {
      G4String datafile = pset->GetBeamParticle() + "_" 
                        + pset->GetTargetMaterial() + "_"
			+ bmom + "_" + sec[is] + "_FW.dat";
      std::ifstream infile;
      infile.open( (datadir+datafile).c_str() );
   
      if ( !infile.is_open() )
      {   
         G4cout << " can NOT open data file " << (datadir+datafile) << G4endl;   
      }
      else
      {
         G4cout << " Open datafile: " << (datadir+datafile) << G4endl;
      }
      
      // POPULATE HERE !!!
      
      for ( int i=0; i<NSetsFW; ++i )
      {
         infile >> AngleBinHARP[0][i] >> AngleBinHARP[1][i];
         infile >> NPointsHARP[i];
      
         //--> G4cout << "Angle Bin: " << AngleBinHARP[0][i] << " " << AngleBinHARP[1][i] << G4endl;
         //--> G4cout << "NPoints: " << NPointsHARP[i] << G4endl;
      
         XMinHARP[i] = new float[NPointsHARP[i]];
         XMaxHARP[i] = new float[NPointsHARP[i]];
         YHARP[i]    = new float[NPointsHARP[i]];
         EYHARP[i]   = new float[NPointsHARP[i]];
         for ( int j=0; j<NPointsHARP[i]; j++ )
         {
            infile >> XMinHARP[i][j] >> XMaxHARP[i][j] >> YHARP[i][j] >> EYHARP[i][j];
	    YHARP[i][j] *= 1000.;  // why do we miltiply by 1000. ? I think it has something with the units (mb,...)
	    EYHARP[i][j] *= 1000.; // there's something in Valdimir's scripts about that...
            //--> G4cout << XMinHARP[i][j] << " " << XMaxHARP[i][j] << " " << YHARP[i][j] << " " << EYHARP[i][j] << G4endl;
	 }
	 // some speactras are pubslished with zero bin content/error (at the end of scale);
	 // for fit purpose those are better be removed
	 //
	 int zero_out = NPointsHARP[i];
	 for ( int j=0; j<NPointsHARP[i]; ++j )
	 {
	    if ( YHARP[i][j] < 1.e-06 ) 
	    {
	       zero_out = j;
	       break;
	    }
	 }
	 //--> G4cout << " zero_out=" << zero_out << G4endl;
	 
	 double* expbins = new double[zero_out+1];
	 for ( int j=0; j<zero_out; ++j )
	 {
	    expbins[j] = XMinHARP[i][j];
	 }
	 expbins[zero_out] = XMaxHARP[i][zero_out-1];
	 
//	 for ( int j=0; j<=zero_out; ++j )
//	 {
//	    G4cout << " j=" << j << " expbins[j]=" << expbins[j] << G4endl;
//	 }
	 
         std::ostringstream osTitle;
         osTitle.clear();
         osTitle << AngleBinHARP[0][i];
	 G4String htitle = ht + " -> " + sec[i] + " X, " + osTitle.str();
	 // this is neede to completely clear out the content
	 // because just the clear() only resets the counter
	 osTitle.str("");
	 osTitle.clear();
	 htitle += ( "<theta<" + osTitle.str() + "(rad)" );
	 G4String exphn = "exp_" + sec[is] + "_FW_" + std::to_string(i);
	 G4String mchn  = "mc_"  + sec[is] + "_FW_" + std::to_string(i); 
	 if ( is == 0 ) // sec.pi-
	 {
	    fExpPiMinusFW.push_back( new TH1D( exphn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    fSimPiMinusFW.push_back( new TH1D( mchn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    for ( int j=0; j<zero_out; ++j )
	    {
	       fExpPiMinusFW.back()->SetBinContent( j+1, YHARP[i][j] );
	       fExpPiMinusFW.back()->SetBinError( j+1, EYHARP[i][j] );
	    }
	 }
	 else if ( is == 1 ) // sec.pi+
	 {
	    fExpPiPlusFW.push_back( new TH1D( exphn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    fSimPiPlusFW.push_back( new TH1D( mchn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    for ( int j=0; j<zero_out-1; ++j )
	    {
	       fExpPiPlusFW.back()->SetBinContent( j+1, YHARP[i][j] );
	       fExpPiPlusFW.back()->SetBinError( j+1, EYHARP[i][j] );
	    }
	 }
	 else if ( is == 2 ) // sec.proton
	 {
	    fExpProtonFW.push_back( new TH1D( exphn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    fSimProtonFW.push_back( new TH1D( mchn.c_str(), htitle.c_str(), zero_out, expbins ) );
	    for ( int j=0; j<zero_out-1; ++j )
	    {
	       fExpProtonFW.back()->SetBinContent( j+1, YHARP[i][j] );
	       fExpProtonFW.back()->SetBinError( j+1, EYHARP[i][j] );
	    }
	 }
	 
	 // cleanup
	 delete [] expbins;
	 
      }
      
      // cleanup
      for ( int i=0; i<NSetsFW; ++i )
      {
         delete [] XMinHARP[i];
	 delete [] XMaxHARP[i];
	 delete [] YHARP[i];
	 delete [] EYHARP[i];
      } 
      
      if ( infile.is_open() ) infile.close();
   }
   
   // cleaup
   if (AngleBinHARP) 
   {
      delete [] AngleBinHARP[0];
      delete [] AngleBinHARP[1];
      delete [] AngleBinHARP;
      AngleBinHARP = 0;
   }    
   if ( XMinHARP )
   {
      delete [] XMinHARP;
      XMinHARP = 0;
   }   
   if ( XMaxHARP )
   {
      delete [] XMaxHARP;
      XMaxHARP = 0;
   }   
   if ( YHARP )
   {
      delete [] YHARP;
      YHARP = 0;
   }   
   if ( EYHARP )
   {
      delete [] EYHARP;
      EYHARP = 0;
   }   

   // LA datasets exist only for sec. pi- and pi+
   
   AngleBinHARP = new float*[2];
   AngleBinHARP[0] = new float[NSetsLA];
   AngleBinHARP[1] = new float[NSetsLA]; 

   for ( int is=0; is<2; ++is )
   {
      G4String datafile = pset->GetBeamParticle() + "_" 
                        + pset->GetTargetMaterial() + "_"
			+ bmom + "_" + sec[is] + "_LA.dat";
      std::ifstream infile;
      infile.open( (datadir+datafile).c_str() );
   
      if ( !infile.is_open() )
      {   
         G4cout << " can NOT open data file " << (datadir+datafile) << G4endl;   
      }
      else
      {
         G4cout << " Open datafile: " << (datadir+datafile) << G4endl;
      }
      
      // POPULATE !!!
      
      if ( infile.is_open() ) infile.close();
   }

   // cleaup
   if (AngleBinHARP) 
   {
      delete [] AngleBinHARP[0];
      delete [] AngleBinHARP[1];
      delete [] AngleBinHARP;
   }    
   if ( XMinHARP )
   {
      delete [] XMinHARP;
      XMinHARP = 0;
   }   
   if ( XMaxHARP )
   {
      delete [] XMaxHARP;
      XMaxHARP = 0;
   }   
   if ( YHARP )
   {
      delete [] YHARP;
      YHARP = 0;
   }   
   if ( EYHARP )
   {
      delete [] EYHARP;
      EYHARP = 0;
   }   

   return;

}

void TestHARPHisto::FillEvt( G4VParticleChange* aChange, const G4LorentzVector&, const G4LorentzVector& ) 
{

   G4int NSec = aChange->GetNumberOfSecondaries();
   
   if ( NSec > 0 ) fHistoNSec->Fill( (double)NSec );
     
   const G4DynamicParticle* sec = 0;
      
   for (G4int i=0; i<NSec; i++) 
   {
        sec = aChange->GetSecondary(i)->GetDynamicParticle();			
	const G4String& pname = sec->GetDefinition()->GetParticleName();
		
	// G4ThreeVector mom = sec->GetMomentum() / GeV ;
	G4double pmom = sec->GetTotalMomentum() / GeV ;
	//double mass = sec->GetDefinition()->GetPDGMass() / GeV;
	//double ekin = sec->GetKineticEnergy() / GeV ;
	G4double theta = (sec->GetMomentum()).theta();
	
//------------------- Mu2e business

	int ipbin = -1; 
	for ( G4int ip=0; ip<fNMomBinsLA; ++ip )
	{
	   if ( pmom > fMomBinsLA[ip] && pmom <= fMomBinsLA[ip+1] )
	   {
	      ipbin = ip;
	      break;
	   }
	}
	if ( ipbin >= 0 && ipbin < fNMomBinsLA ) 
	{
	   if ( pname == "pi-" )
	   {
	      fHistoSecPiMinusMomBins[ipbin]->Fill( theta );
	   }
	   else if ( pname == "pi+" )
	   {
	      fHistoSecPiPlusMomBins[ipbin]->Fill( theta );
	   }
	}
//---------------------

	if ( theta < fThetaMinFW ) continue;
	if ( theta < fThetaMinFW+fDeltaThetaFW*fNThetaBinsFW )
	{
	   G4int ith = ( theta - fThetaMinFW ) / fDeltaThetaFW;
	   if ( pname == "pi-" )
	   {
	      fHistoSecPiMinusFW[ith]->Fill( pmom );
	      if ( fIncludeExpData ) fSimPiMinusFW[ith]->Fill( pmom );
	   }
	   else if ( pname == "pi+" )
	   {
	      fHistoSecPiPlusFW[ith]->Fill( pmom );
	      if ( fIncludeExpData ) fSimPiPlusFW[ith]->Fill( pmom );
	   }
	   else if ( pname == "proton" )
	   {
	      fHistoSecProtonFW[ith]->Fill( pmom );
	      if ( fIncludeExpData ) fSimProtonFW[ith]->Fill( pmom );
	   }
	}
		
	if ( theta < fThetaMinLA ) continue;
	if ( theta > fThetaMinLA+fDeltaThetaLA*fNThetaBinsLA ) continue;
	G4int    itheta = ( theta - fThetaMinLA ) / fDeltaThetaLA;
	if ( itheta < 0 || itheta >= fNThetaBinsLA ) continue;
	        
	if ( pname == "pi-" )
	{
	   fHistoSecPiMinusLA[itheta]->Fill( pmom );
	}
	else if ( pname == "pi+" )
	{
	   fHistoSecPiPlusLA[itheta]->Fill( pmom );
	}
	else if ( pname == "proton" )
	{
	   fHistoSecProtonLA[itheta]->Fill( pmom );
	}
   }
      
   return;
   
}

void TestHARPHisto::Write( G4int stat, G4double xsec )
{

   G4double xbin = 1.;
   G4double scale = 1.;
      
   xbin = (G4double)fHistoNSec->GetBinWidth(1);
   scale = 1. / (xbin*stat);
   fHistoNSec->Scale( scale ) ;
   fHistoNSec->Write();
   
   // secondary pi-
   //
   for ( size_t i=0; i<fHistoSecPiMinusFW.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaFW);
      fHistoSecPiMinusFW[i]->Scale( scale, "width" );
      fHistoSecPiMinusFW[i]->Write();
   }
   for ( size_t i=0; i<fHistoSecPiMinusLA.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaLA);
      fHistoSecPiMinusLA[i]->Scale( scale, "width" );
      fHistoSecPiMinusLA[i]->Write();
   }

   // secondary pi+
   //
   for ( size_t i=0; i<fHistoSecPiPlusFW.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaFW);
      fHistoSecPiPlusFW[i]->Scale( scale, "width" );
      fHistoSecPiPlusFW[i]->Write();
   }
   for ( size_t i=0; i<fHistoSecPiPlusLA.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaLA);
      fHistoSecPiPlusLA[i]->Scale( scale, "width" );
      fHistoSecPiPlusLA[i]->Write();
   }
   
   // secondary proton
   //
   for ( size_t i=0; i<fHistoSecProtonFW.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaFW);
      fHistoSecProtonFW[i]->Scale( scale, "width" );
      fHistoSecProtonFW[i]->Write();
   }
   for ( size_t i=0; i<fHistoSecProtonLA.size(); ++i )
   {
      scale = xsec / (stat*fDeltaThetaLA);
      fHistoSecProtonLA[i]->Scale( scale, "width" );
      fHistoSecProtonLA[i]->Write();
   }
   
//-------------------- Mu2 business

   for ( size_t i=0; i<fHistoSecPiMinusMomBins.size(); ++i )
   {
      scale = xsec / (stat*(fMomBinsLA[i+1]-fMomBinsLA[i]));
      fHistoSecPiMinusMomBins[i]->Scale( scale, "width" );
      fHistoSecPiMinusMomBins[i]->Write();
   }
   for ( size_t i=0; i<fHistoSecPiPlusMomBins.size(); ++i )
   {
      scale = xsec / (stat*(fMomBinsLA[i+1]-fMomBinsLA[i]));
      fHistoSecPiPlusMomBins[i]->Scale( scale, "width" );
      fHistoSecPiPlusMomBins[i]->Write();
   }

   if ( !fIncludeExpData ) return;
   
   // now make subdiretory for Exp&MC, if fIncludeExpData=true(1)
   //
   
   if ( ! fHistoNSec->GetDirectory() )
   {
      G4cout << " Can NOT find directory/file the histogramms belong to; return" << G4endl;
      return;   
   }

   TFile* fout = fHistoNSec->GetDirectory()->GetFile();
   
   G4cout << " Histogramms written to file: " << fout->GetName() << G4endl;
   
   TDirectory* dir = fout->mkdir("exp_and_mc_histo");
   dir->cd();  
   
   // NOTE: there maybe better idea of either storing theta-bins from data
   //       or (less faborable) extracting them from the histo title
   //
   // NOTE: do NOT scale exp.histo !!!
   //
   scale = xsec / (stat*fDeltaThetaFW);
   for ( size_t i=0; i<fExpPiMinusFW.size(); ++i )
   {
      fExpPiMinusFW[i]->Write();
   }
   for ( size_t i=0; i<fExpPiPlusFW.size(); ++i )
   {
      fExpPiPlusFW[i]->Write();
   }
   for ( size_t i=0; i<fExpProtonFW.size(); ++i )
   {
      fExpProtonFW[i]->Write();
   }
   for ( size_t i=0; i<fSimPiMinusFW.size(); ++i )
   {
      fSimPiMinusFW[i]->Scale( scale, "width" );
      fSimPiMinusFW[i]->Write();
   }
   for ( size_t i=0; i<fSimPiPlusFW.size(); ++i )
   {
      fSimPiPlusFW[i]->Scale( scale, "width" );
      fSimPiPlusFW[i]->Write();
   }
   for ( size_t i=0; i<fSimProtonFW.size(); ++i )
   {
      fSimProtonFW[i]->Scale( scale, "width" );
      fSimProtonFW[i]->Write();
   }
 
   
   return;

}
