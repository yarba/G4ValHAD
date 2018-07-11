std::string TEST_NAME="test19";
#include "../test23/shared-root-macros/REGRESSION_TEST.h"

#include "../test19/G4MODELS_HighEnergy.h"

#include "../test23/shared-root-macros/ReadNA49Data.C"

#include "../test23/shared-root-macros/Chi2Calc.C"
#include "../test23/shared-root-macros/Chi2CalcNA49.C"

#include <iostream>
#include <fstream>

void NA49_chi2_table_regre( std::string beam, std::string target, std::string model )
{

   std::vector< std::pair<double,int> > dN_dxF_piplus;
   std::vector< std::pair<double,int> > pT_dxF_piplus;
   std::vector< std::pair<double,int> > d2sigma_dxFdpT_piplus;
   
   std::vector< std::pair<double,int> > dN_dxF_piminus;
   std::vector< std::pair<double,int> > pT_dxF_piminus;
   std::vector< std::pair<double,int> > d2sigma_dxFdpT_piminus;

   std::vector< std::pair<double,int> > dN_dxF_proton;
   std::vector< std::pair<double,int> > pT_dxF_proton;
   std::vector< std::pair<double,int> > d2sigma_dxFdpT_proton;
   
   std::vector< std::pair<double,int> > dN_dxF_antiproton;
   std::vector< std::pair<double,int> > pT_dxF_antiproton;
   std::vector< std::pair<double,int> > d2sigma_dxFdpT_antiproton;

   std::vector< std::pair<double,int> > dN_dxF_neutron;
   
   // piplus
   //
   
   readIntegratedSpectra( beam, target, "piplus" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, "piplus", "dNdxF", model, NDF, Versions[iv] );
      dN_dxF_piplus.push_back( std::pair<double,int>(chi2,NDF) );
      NDF = 0;
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, "piplus", "pT", model, NDF, Versions[iv] );
      pT_dxF_piplus.push_back( std::pair<double,int>(chi2,NDF) );
   }
   
   readDDiffSpectra( beam, target, "piplus" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2DDiffXSecNA49( beam, target, "piplus", model, NDF, Versions[iv] );
      d2sigma_dxFdpT_piplus.push_back( std::pair<double,int>(chi2,NDF) );
   } 
   
   // piminus
   //

   readIntegratedSpectra( beam, target, "piminus" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, "piminus", "dNdxF", model, NDF, Versions[iv] );
      dN_dxF_piminus.push_back( std::pair<double,int>(chi2,NDF) );
      NDF = 0;
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, "piminus", "pT", model, NDF, Versions[iv] );
      pT_dxF_piminus.push_back( std::pair<double,int>(chi2,NDF) );
   }
     
   readDDiffSpectra( beam, target, "piminus" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2DDiffXSecNA49( beam, target, "piminus", model, NDF, Versions[iv] );
      d2sigma_dxFdpT_piminus.push_back( std::pair<double,int>(chi2,NDF) );
   } 
   
   // proton
   //

   readIntegratedSpectra( beam, target, "proton" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, "proton", "dNdxF", model, NDF, Versions[iv] );
      dN_dxF_proton.push_back( std::pair<double,int>(chi2,NDF) );
      NDF = 0;
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, "proton", "pT", model, NDF, Versions[iv] );
      pT_dxF_proton.push_back( std::pair<double,int>(chi2,NDF) );
   }

   readDDiffSpectra( beam, target, "proton" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2DDiffXSecNA49( beam, target, "proton", model, NDF, Versions[iv] );
      d2sigma_dxFdpT_proton.push_back( std::pair<double,int>(chi2,NDF) );
   } 
   
   // antiproton
   //

   readIntegratedSpectra( beam, target, "antiproton" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, "antiproton", "dNdxF", model, NDF, Versions[iv] );
      dN_dxF_antiproton.push_back( std::pair<double,int>(chi2,NDF) );
      NDF = 0;
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, "antiproton", "pT", model, NDF, Versions[iv] );
      pT_dxF_antiproton.push_back( std::pair<double,int>(chi2,NDF) );
   }

/* FIXME !!! NEEDS TO BE ADDED LATER !!!
   readDDiffSpectra( beam, target, "antiproton" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2DDiffXSecNA49( beam, target, "antiproton", model, NDF, Versions[iv] );
      d2sigma_dxFdpT_antiproton.push_back( std::pair<double,int>(chi2,NDF) );
   } 
*/   
   // neutron
   //
   
   readIntegratedSpectra( beam, target, "neutron" );
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, "neutron", "dNdxF", model, NDF, Versions[iv] );
      dN_dxF_neutron.push_back( std::pair<double,int>(chi2,NDF) );
   }

    // PRINT THE FORMATTED MAP INTO 4x4 TABLE !!!
   
   std::ofstream out_table;
   std::string table_name = model +"_" + beam + "_" + target + "_NA49.txt"; 
   
   std::cout << " Opening ascii file: " << table_name << std::endl;

   out_table.open( table_name.c_str() );
     
   out_table << std::right << std::setw(30) << model << ":  158GeV/c " << beam << " on " << target << " --> hadrons" << std::endl;
   out_table << std::right << std::setw(62) << " chi2/NDF calculated vs NA49 data " << std::endl;
   out_table << " " << std::endl;

   out_table << std::right << std::setw(30) << "dN / dxF" << std::right << std::setw(45) << "<pt> vs xF" << std::endl;
   for ( int ic=0; ic<2; ++ic )
   {
   out_table << "           ";
      for ( size_t iv=0; iv<NVersions; ++iv )
      {
         std::string ver = Versions[iv];
         if ( ver.find("geant4-") != std::string::npos )
         {
            ver.erase( 0, std::string("geant4-").length() );
         }
         if ( ver.find( "patch-") != std::string::npos )
         {
            ver.replace( ver.find("patch-"), std::string( "patch-").length(), "p" ); 
         }
         if ( ver.find( "beta-") != std::string::npos )
         {
            ver.replace( ver.find("beta-"), std::string("beta-").length(), "b" );
         }
         while ( ver.find("-") != std::string::npos )
         {
            ver.replace( ver.find("-"), std::string("-").length(), "." );
         }
         out_table << std::setw(12) << ver;
      }
   }

   out_table << "\n pi+       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << dN_dxF_piplus[iv].first/dN_dxF_piplus[iv].second ;
   }   
   out_table << "           ";
   for ( int iv=0; iv<NVersions; ++iv )
   {
      out_table << setw(12) << pT_dxF_piplus[iv].first/pT_dxF_piplus[iv].second ;
   }
   out_table << "\n pi-       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << dN_dxF_piminus[iv].first/dN_dxF_piminus[iv].second ;
   }   
   out_table << "           ";
   for ( int iv=0; iv<NVersions; ++iv )
   {
      out_table << setw(12) << pT_dxF_piminus[iv].first/pT_dxF_piminus[iv].second ;
   }
   out_table << "\n proton    "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << dN_dxF_proton[iv].first/dN_dxF_proton[iv].second ;
   }   
   out_table << "           ";
   for ( int iv=0; iv<NVersions; ++iv )
   {
      out_table << setw(12) << pT_dxF_proton[iv].first/pT_dxF_proton[iv].second ;
   }
   out_table << "\n antiproton"; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << dN_dxF_antiproton[iv].first/dN_dxF_antiproton[iv].second ;
   }   
   out_table << "           ";
   for ( int iv=0; iv<NVersions; ++iv )
   {
      out_table << setw(12) << pT_dxF_antiproton[iv].first/pT_dxF_antiproton[iv].second ;
   }
   out_table << "\n neutron   "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << dN_dxF_neutron[iv].first/dN_dxF_neutron[iv].second ;
   }   

   out_table << "\n " << std::endl;

   out_table << std::right << std::setw(40) << "d2sigma / dxF dpT" << std::endl;
   out_table << "           ";
   for ( size_t iv=0; iv<NVersions; ++iv )
      {
         std::string ver = Versions[iv];
         if ( ver.find("geant4-") != std::string::npos )
         {
            ver.erase( 0, std::string("geant4-").length() );
         }
         if ( ver.find( "patch-") != std::string::npos )
         {
            ver.replace( ver.find("patch-"), std::string( "patch-").length(), "p" ); 
         }
         if ( ver.find( "beta-") != std::string::npos )
         {
            ver.replace( ver.find("beta-"), std::string("beta-").length(), "b" );
         }
         while ( ver.find("-") != std::string::npos )
         {
            ver.replace( ver.find("-"), std::string("-").length(), "." );
         }
         out_table << std::setw(12) << ver;
   }

   out_table << "\n pi+       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << d2sigma_dxFdpT_piplus[iv].first/d2sigma_dxFdpT_piplus[iv].second ;
   }   
   out_table << "\n pi-       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << d2sigma_dxFdpT_piminus[iv].first/d2sigma_dxFdpT_piminus[iv].second ;
   }   
   out_table << "\n proton    ";
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << d2sigma_dxFdpT_proton[iv].first/d2sigma_dxFdpT_proton[iv].second ;
   }   
   out_table << "\n proton           will be added shortly !!!";



   return;

}
