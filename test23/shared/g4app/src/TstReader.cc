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

#include "TstReader.hh"

#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"

#include "G4ParticleTable.hh"

#include <boost/algorithm/string.hpp>

TstReader::TstReader()
   : fInStream(0),fParamStream(0),fEndConfig(false),
     fNEvents(1000), 
     fBeamPart("proton"),
     fBeamEnergy(0.),
     fBeamEKin(0.),
     fBeamMomentum(1000.*MeV),
     fDirection( G4ThreeVector(0.,0.,1.) ),
     fPosition( G4ThreeVector(0.,0.,0.) ),
     fTime(0.),
// new - for test47
//
     fdpByp(0.),
     fAngleX(0.), fdAngleX(0.), fAngleY(0.), fdAngleY(0.),
     fBeamSigmaX(0.), fBeamSigmaY(0.),
     fBeamKineByEvt(false),    
//
     fTargetMaterial("C"),
     fTargetSize( G4ThreeVector(100., 100., 100. ) ),
     fTargetShape("G4Box"),
     fPhysics(""),
     fBertiniAs11_2(false),
     fFTFTuneID(0),
     fPhysicsConfig("default"),
     fStep(0.01*micrometer),
     fRndmSeed(135799753),
     fJobID(-1),
     fClusterID(-1),
     fVerbose(-1),
     fExpDataSet("None"),
     fIncludeExpData(false),
     fForceResDecay(false)    
{
   
   fInStream = new std::ifstream();

}

TstReader::~TstReader()
{

   if ( fInStream )
   {
      if ( fInStream->is_open() ) fInStream->close();
      delete fInStream;
      fInStream = nullptr;
   }
   if ( fParamStream )
   {
      if (fParamStream->is_open() ) fParamStream->close();
      delete fParamStream;
      fParamStream = nullptr;
   }

}

void TstReader::Help() 
{

   G4cout << "Available configuration commands are: " << G4endl;
   G4cout << "#events" << G4endl;
   G4cout << "#particle" << G4endl;
   G4cout << "#energy(MeV)" << G4endl;
   G4cout << "#momentum(MeV/c)" << G4endl;
   G4cout << "#step(mm)" << G4endl;
   G4cout << "#material" << G4endl;
   G4cout << "#target-geom(mm)" << G4endl;
   G4cout << "#generator" << G4endl;
   G4cout << "#verbose" << G4endl;
   G4cout << "#position(mm)" << G4endl;
   G4cout << "#direction" << G4endl;
   G4cout << "#time(ns)" << G4endl; // why would I need this ?
   //
   G4cout << "#isHARP" << G4endl;
   G4cout << "#isNA61" << G4endl;
   G4cout << "#isNA49" << G4endl;
   G4cout << "#isSASM6E" << G4endl;
   G4cout << "#isXFPlots" << G4endl;
   // G4cout << "isMIPP" << G4endl;
   G4cout << "#isITEP" << G4endl;
   // G4cout << "isBNL" << G4endl;
   //
   G4cout << "#includeExpData" << G4endl;
   //
   G4cout << "#forceResonanceDecay" << G4endl;
  
//
// for parallel processing
//
   G4cout << "#randomSeed" << G4endl;
   G4cout << "#jobID" << G4endl;
   G4cout << "#clusterID" << G4endl;

   G4cout << "#run" << G4endl;
   G4cout << "#exit" << G4endl;

   return;

}

void TstReader::OpenAppConfig( std::string conf )
{

   fInStream->open( conf.c_str() );
   if ( !fInStream->is_open() )
   {
    G4cout << "Input file <" << conf << "> does not exist! Exit" << G4endl;
    exit(1);
   }   

   return;

}

void TstReader::CloseAppConfig()
{
   
   if ( fInStream->is_open() ) fInStream->close();
   if ( !fParamStream )
   {
      if ( fParamStream->is_open() ) fParamStream->close();
   }
   
   return;
   
}

std::vector<std::string> TstReader::GetConfiguredModels() const
{

   std::vector<std::string> models;

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.begin();
   for ( ; itr!=fModelConfig.end(); ++itr )
   {
      models.push_back( itr->first );
   }
   
   return models;

}

const std::vector< std::pair<std::string,double> >* TstReader::GetConfig( const std::string& model ) const
{

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.find(model);
   
   if ( itr == fModelConfig.end() )
   {
      std::cout << " There is NO NON-DEFAULT CONFIGURATION for model " << model  
                << " in the container; NULL returned" << std::endl;
      return NULL;
   }   

   return &(itr->second);
   
}

void TstReader::ProcessConfig()
{

   G4String line;
   
   do
   {
      (*fInStream) >> line;
      if ( line == "#exit" )
      {
         fEndConfig = true;
	 break;
      }
      else if ( line == "#run" )
      {
         break;
      }
      else
      {
         ProcessLine( line );
	 // check if phys.config is or isn't "default"
         G4String physconfig = GetPhysicsConfig();
         G4String physconfig_lc =  boost::algorithm::to_lower_copy( physconfig );
   
         // do nothing is default model configuration
         //
         if ( physconfig_lc.find("default") != std::string::npos ) continue;
	 // non-default; process parameter file
	 ProcessModelParameters();
      }
   
   } while(!fEndConfig);
   
   // can't call it here because G4ParticleTable is still empty...
   //
   // SyncKinematics();
   
   return;

}

void TstReader::ProcessModelParameters()
{

   // non-default model configuration; read-in config file
   //
   // first of all, find any env.variables in the path to config
   // and convert to full path
   //
   G4String tmp_physconfig = GetPhysicsConfig();
   size_t start_pos = 0;
   size_t found_pos = tmp_physconfig.find( "$", start_pos );
   
   while ( found_pos != std::string::npos )
   {
      size_t found_slash = tmp_physconfig.find( "/", found_pos );
      G4String envvar_name = "";
      if ( found_slash != std::string::npos )
      {
	 envvar_name = tmp_physconfig.substr( found_pos+1, (found_slash-found_pos-1) );
      }
      else
      {
	 envvar_name = tmp_physconfig.substr( found_pos+1, (tmp_physconfig.length()-found_pos-1) );
      }
// -->      std::cout << " env.var. = " << envvar_name << std::endl;
      // expand env.var. into full path
      //
      G4String fullpath_envvar = getenv( envvar_name.c_str() );
// -->      std::cout << " full path = " << fullpath_envvar << std::endl;
      //
      // NOTE(JVY): need to find a way to remove the leading slash 
      //            if **not the 1st** occurance of env.var. in the path,
      //            unless "double slash" in the path can be processed OK 
      //
      tmp_physconfig.replace( found_pos, (found_slash-found_pos), fullpath_envvar );
// -->      std::cout << " updated tmp_physconfig = " << tmp_physconfig << std::endl;
      found_pos = tmp_physconfig.find( "$", /* found_pos+1 */ fullpath_envvar.length() );
   }

   // now that the full path to config file is known explicitly,
   // open the file for reading
   //
   fParamStream = new std::ifstream();
   fParamStream->open( tmp_physconfig.c_str() );
   if ( !fParamStream->is_open() ) 
   {
      std::cout << " Application config indicates custom configuration for " 
                << GetPhysics() << " but the parameter file is NOT found" << std::endl;
      std::cout << " EXIT !" << std::endl;
      exit(1);
   }
   else
   {
      std::cout << " Model configuration file is successfully open" << std::endl;
   }

   // from here on, read each line, process it, find/match model config. instructions, etc...
   //
   // NOTE(JV): the loop while(fParamStream->eof()) {...} will execute one time too many !
   //           that's because the eofbit status flag is set only after a read operation 
   //           encounters an empty return
   //
   G4String line;
   G4String current_model = "";
   G4String whitespace = " \t\r\n\v\f";

   while ( std::getline( (*fParamStream), line ) )
   {
      // process line once it's in
      //
            
      // skip comment segment(s) and/or (part of) line(s)
      // that'd be either a fragment within /*... */ or
      // portion of a line after double slash (i.e. //)
      //
      if ( line.find("/*") != std::string::npos ) // found leading /* of a comment block 
      {
         if ( line.find("*/") != std::string::npos ) // found closing */ of the comment block
	                                             // on the SAME line 
	 {
	    std::getline( (*fParamStream), line ); // just read the next line in this case
	 }
	 else // keep reading until */ is found
	 {
	    while ( line.find("*/") == std::string::npos  ) 
	    {
	       std::getline( (*fParamStream), line );
	    }
	    std::getline( (*fParamStream), line ); // read-in one more time;
	                                           // otherwise it'll stay at */
	 }
      }
      
      if ( line.find("//") != std::string::npos ) 
      {
         size_t comment_pos = line.find("//");
	 std::cout << " in line " << line << " comment_pos = " << comment_pos << std::endl;
	 if ( comment_pos == 0 ) // check if at the start; in this case skip the whole line
	 {
	    continue;
	 }
	 else   // remove the comment portion of the line (after //)
	 {
	    line.erase( comment_pos );
	    std::cout << " updated line: " << line << std::endl;
	 }
      }
      
      // skip empty line(s) and/or line(s) containing only whitespace(s)
      //
      if ( line.empty() || line.find_first_not_of(whitespace /*" \t\r\n\v\f"*/) == std::string::npos ) continue;
      
      // OK, eligible line (non-comment)
      //
      // PROCESS AND STORE to ModelConfig for later use !!!
      //
      std::cout << "new line: " << line << std::endl;
      
      std::map< std::string, std::vector< std::pair<std::string,double> > >::iterator itr_model;
      if ( line.find("#model") != std::string::npos )
      {
         // get next line
	 // somehow if I use getline here it later bombs in InitBeam; WHY ???
	 // std::getline( (*fParamStream), line );
	 (*fParamStream) >> current_model; // line;
	 fParamStream->ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	 std::cout << " current_model = " << current_model << std::endl;
	 boost::algorithm::to_lower( current_model );
	 std::cout << " current_model (lower case) = " << current_model << std::endl;
	 // now insert the model name in fModelConfig map
	 //
         if ( fModelConfig.find(current_model) == fModelConfig.end() )
         {
            fModelConfig.insert( 
	       std::pair< std::string, 
               std::vector< std::pair<std::string,double> > >( current_model,										                                               std::vector< std::pair<std::string,double> >() ) );
         }
      }
      if ( line.find("#parameter_set") != std::string::npos )
      {
	 while( line.find("#end_of_parameter_set") == std::string::npos )
	 {
	    std::getline( (*fParamStream), line );
	    if ( line.find("parameter_set") != std::string::npos ) continue;
	    // we get here if parameter line 
	    itr_model = fModelConfig.find(current_model);	 
	    if ( itr_model != fModelConfig.end() )
	    {
	       // OK, model (name) is already in the map
	       // break the param config line into tokens (param name and value) & insert into map
	       size_t delim_pos = line.find(":");
	       G4String pname = line.substr( 0, delim_pos-1 );
	       pname.erase( std::remove(pname.begin(),pname.end(),' '), pname.end() );
	       G4String pvalue = line.substr( delim_pos+1, line.length() );
	       pvalue.erase( std::remove(pvalue.begin(),pvalue.end(),' '), pvalue.end() );
	       G4double value = std::stod( pvalue );
	       std::cout << " for model " << itr_model->first << 
	                    " about to insert pname = " << pname << " value = " << value << std::endl;
               (itr_model->second).push_back( std::pair<std::string,double>(pname,value) );	       
	    }
	 }
      }

   }
      
   return;

}

void TstReader::SyncKinematics() const
{
   
   G4ParticleDefinition* partDef = (G4ParticleTable::GetParticleTable())->FindParticle(fBeamPart);      
   G4double partMass = partDef->GetPDGMass();   
   // G4double partMass = (G4ParticleTable::GetParticleTable())->FindParticle(fBeamPart)->GetPDGMass();
   fBeamEnergy = sqrt( fBeamMomentum*fBeamMomentum + partMass*partMass );   
   fBeamEKin = fBeamEnergy - partMass;

   return;

}

void TstReader::ProcessLine( G4String line )
{

      std::string line1;
      G4double nx, ny, nz;
      
      if(line == "#particle") 
      {
        (*fInStream) >> fBeamPart;
      } 
      else if(line == "#momentum(MeV/c)") 
      {
        (*fInStream) >> fBeamMomentum;
        fBeamMomentum *= MeV;
      } 
      else if(line == "#events") 
      {
        (*fInStream) >> line1;
        std::istringstream is(line1);
        is >> fNEvents;
        // G4cout << "nevt : " << nevt << G4endl;
      } 
      else if(line == "#step(mm)") 
      {
        (*fInStream) >> fStep;
        fStep *= mm;
      } 
      else if(line == "#material") 
      {
        (*fInStream) >> fTargetMaterial;
      } 
      else if ( line == "#target-geom(mm)" )
      {
         (*fInStream) >> nx >> ny >> nz >> fTargetShape;
	 fTargetSize = G4ThreeVector(nx*mm, ny*mm, nz*mm);
      }
      else if(line == "#verbose") 
      {
        (*fInStream) >> fVerbose;
      } 
      else if(line == "#position(mm)") 
      {
	(*fInStream) >> nx >> ny >> nz;
        fPosition = G4ThreeVector(nx*mm, ny*mm, nz*mm);
      } 
      else if(line == "#direction") 
      {
        (*fInStream) >> nx >> ny >> nz;
        if( (nx*nx+ny*ny+nz*nz) > 0.0) 
	{
          fDirection = G4ThreeVector(nx, ny, nz);
          fDirection = fDirection.unit();
	}
      } 
      else if(line == "#time(ns)") 
      {
        (*fInStream) >> fTime;
        fTime *= ns;
      } 
//
// Exp. Data Set type
//
      else if ( line == "#isNA49" )
      {
         SetExpDataSet( "NA49" );
      }
      else if ( line == "#isNA61" )
      {
         SetExpDataSet( "NA61" );
      } 
      else if ( line == "#isHARP" )
      {
         SetExpDataSet( "HARP" );
      }
      else if ( line == "#isSASM6E" )
      {
         SetExpDataSet( "SASM6E" );
      }
      else if ( line == "#isIAEA" )
      {
         SetExpDataSet( "IAEA" );
      }
      else if ( line == "#isXFPlots" )
      {
         SetExpDataSet( "xFplots" );
      }
/*
      else if ( line == "#isITEP" )
      {
         SetExpDataSet( "ITEP" );
      }
      else if ( line == "#isBNL" )
      {
         SetExpDataSet( "BNL" );
      }
      else if ( line == "#isMIPS" )
      {
         SetExpDataSet( "MIPS" );
      }
*/
      else if ( line == "#includeExpData" )
      {
         SetIncludeExpData( true );
      }
//
// needed for parallel processing
//
      else if ( line == "#randomSeed" )
      {
        (*fInStream) >> fRndmSeed;
      }
      else if ( line == "#jobID" )
      {
	(*fInStream) >> fJobID ;
      }
      else if ( line == "#clusterID" )
      {
	(*fInStream) >> fClusterID ;
      }
      else if ( line == "#forceResonanceDecay" )
      {
         fForceResDecay = true;
      }

// mainly for test47...
//
      else if ( line == "#dpByp" ) 
      {
         (*fInStream) >> fdpByp;
	 fBeamKineByEvt = true;
      }
      else if ( line == "#angleX" )
      {
         (*fInStream) >> fAngleX >> fdAngleX;
	 fAngleX  *= mrad;
	 fdAngleX *= mrad;
      }
      else if ( line == "#angleY" )
      {
         (*fInStream) >> fAngleY >> fdAngleY;
	 fAngleY  *= mrad;
	 fdAngleY *= mrad;
	 fBeamKineByEvt = true;
      }
      else if ( line == "#beam-spread" )
      {
         (*fInStream) >> fBeamSigmaX >> fBeamSigmaY;
	 fBeamSigmaX *= mm;
	 fBeamSigmaY *= mm;
      }

   return;

}
