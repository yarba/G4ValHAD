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

#include "TstDiscreteProcessReader.hh"
#include <limits>

void TstDiscreteProcessReader::ProcessLine( G4String line )
{
   
   TstReader::ProcessLine( line );
   
   if ( line != "#generator" ) return;
   
   if(line == "#generator") 
   {
        // (*fInStream) >> fPhysics; // the >> operator stops at the 1st whitespace;
	                             // but it also automatically goes to the next line
	// this is needed because otherwise getline will read the leftover remainder of the current line
	
	fInStream->ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	std::getline( *fInStream, fPhysics ); // this one reads the whole line

/* it's operational but won't account if there're extra whitespaces
	std::string::size_type pos = fPhysics.find(":");
	if ( pos != std::string::npos )
	{
	   fPhysicsConfig = fPhysics.substr(pos+2,fPhysics.length());
	   fPhysics = fPhysics.substr(0,pos-1);
	}
*/	
	size_t delim_pos = fPhysics.find(":"); // one can also use rfind
        if ( delim_pos != std::string::npos )
	{
	   G4String whitespace = " \t\n\r\f\v";
	   size_t char_pos = fPhysics.find_first_not_of( whitespace, delim_pos+1 );
	   fPhysicsConfig = fPhysics.substr( char_pos, fPhysics.length() );	
	   // NOTE(JVY): if delim_pos instead of delim_pos-1, it doesn't skip whitespaces
	   char_pos = fPhysics.find_last_not_of( whitespace, delim_pos-1 );
	   // if char_pos instead of char_pos+1 it chops the last char (to add endl?)
	   fPhysics = fPhysics.substr( 0, char_pos+1 );	
	}
	
	// check/remove if there's any whitespace accidently left 
	// in the name of physics model/config
	fPhysics.erase( std::remove(fPhysics.begin(),fPhysics.end(),' '), 
	                fPhysics.end() );
	fPhysicsConfig.erase( std::remove(fPhysicsConfig.begin(),fPhysicsConfig.end(),' '), 
	                      fPhysicsConfig.end() );
	
	std::cout << " physics model = " << fPhysics 
	          << " config = " << fPhysicsConfig 
	<< std::endl;
   }

   return;

}
