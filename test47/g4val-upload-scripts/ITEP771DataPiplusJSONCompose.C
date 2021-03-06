/*
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
*/

#include <math.h>
#include <vector>

#include "TSystem.h"
#include "Rtypes.h"

#include "../PlotITEPAnalysis.C"

#include "DumpITEP771.C"

void ITEP771DataJSONCompose( std::ofstream&, std::string, int, std::string );

void ITEP771DataJSONMaster( std::string tgt, int tgtlnk )
{

   std::string fjsonname = "ITEP771-spectra-piplus-";
   fjsonname += tgt;
   fjsonname += ".json";
   
   std::ofstream jsonfile;
   jsonfile.open( fjsonname.c_str(), ios::ate|ios::app ); 
   
   jsonfile << "{\"ResultList\":[" << std::endl;
   
   ITEP771DataJSONCompose( jsonfile, tgt, tgtlnk, "proton" );
      
   ITEP771DataJSONCompose( jsonfile, tgt, tgtlnk, "neutron" );
      
   jsonfile << "]}" << std::endl;
    
   return;

}

void ITEP771DataJSONCompose( std::ofstream& jsonfile, std::string tgt, int tgtlnk, std::string secondary )
{
   
   // NOTE: We have exp.data only up to 6GeV/c for pi+ beam

   std::string sec = secondary;
   if ( secondary == "piplus" )
   {
      sec = "pi+";
   }
   else if ( secondary == "piminus" )
   {
      sec = "pi-";
   }
   
   int seclink = -1;
   if ( secondary == "proton" )
   {
      seclink = 2212;
   }
   else if ( secondary == "neutron" )
   {
      seclink = 2112;
   } 
   
   std::string angles[4] = { "59.1", "89.0", "119.0", "159.6" };

   //
   // 1GeV/c pi+ on A
   //

   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "1.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=61 is 1 GeV/c pi+ 
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":61,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 1GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }
   
   // 1.4GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "1.40", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=62 is 1.4 GeV/c pi+ 
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":62,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 1.4GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }
   
   // 2GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "2.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=63 is 2 GeV/c pi+ 
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":63,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 2GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }

   // 3GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "3.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=42 is 3 GeV/c pi+ (overlap with HARP !!!) 
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":42,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 3GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }

   // 4GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "4.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=64 is 4 GeV/c pi+  
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":64,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 4GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }
   
   // 5GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "5.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=43 is 5 GeV/c pi+ (overlap with HARP !!!)
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":43,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 5GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }
   
   // 6GeV/c pi+ on A
   //
   for ( int ia=0; ia<4; ++ia )
   {
   
      bool ok = readKEData( "piplus", tgt, "6.00", secondary, angles[ia] ); 
   
      if ( ok )
      {      
         jsonfile << "{" << std::endl;
      
         // NOTE: referencelnk=17 Yu.D.Bayukov in Sov.J.Nucl.Phys 1985 (in English)
         //
         jsonfile << "\"trid\":1,\"testlnk\":0,\"referencelnk\":17,\"mcdetaillnk\":1," << std::endl;
   
         // NOTE: observablelnk=9 means Lorentz-inv E(d3sigma/dp3)
         //
         // NOTE: beamlnk=65 is 6 GeV/c pi+
         //
         // NOTE: reactionlnk=1 means "particle production"
         //
         jsonfile << "\"beamlnk\":65,\"targetlnk\":"<< tgtlnk <<",\"observablelnk\":9,\"secondarylnk\":" 
                  << seclink << ",\"reactionlnk\":1," << std::endl;

         jsonfile << "\"datatable\":{" << std::endl; // start datatable
      
         jsonfile << "\"dtid\":1,\"datatypeslnk\":1," << std::endl;
         jsonfile << "\"title\":\"Production of " << sec << " in pi+ on "<< tgt << " interactions at 6GeV/c, theta=" 
	          << angles[ia] << " [deg]\"," << std::endl;
         jsonfile << "\"npoints\":0," << std::endl;
         jsonfile << "\"nbins\":[" << NPtKE << "]," << std::endl;
         jsonfile << "\"axisTitle\":[\"kinetic energy of " << sec << " [GeV]\",\"E(d3sigma/dp3) [mb/GeV**2]\"]," << std::endl;
      
         DumpITEP771( jsonfile, secondary );

         jsonfile << "}," << std::endl; // end of datatable 
      
// --->         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1,\"parvalue\":{}" << std::endl;
         jsonfile << "\"imageblobslnk\":NULL,\"scoreslnk\":2,\"accesslnk\":1," << std::endl; 
	 jsonfile << "\"parnames\":[\"theta [deg] of secondary particle\"],\"parvalues\":[\"" << angles[ia]
	          << "\"]" << std::endl;

         jsonfile << "}," << std::endl; 
      }
   }
         
   return;

}

