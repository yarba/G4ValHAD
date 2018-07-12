std::string TEST_NAME="test19";
#include "../test23/shared-root-macros/REGRESSION_TEST.h"

#include "../test19/G4MODELS_HighEnergy.h"

#include "../test23/shared-root-macros/ReadNA61Data.C"

#include "../test23/shared-root-macros/Chi2Calc.C"
#include "../test23/shared-root-macros/Chi2CalcNA61.C"

#include <iostream>
#include <fstream>

void NA61_chi2_table_regre( std::string beam, std::string target, std::string model )
{

   std::vector< std::pair<double,int> > piplus;
   std::vector< std::pair<double,int> > piminus;
   std::vector< std::pair<double,int> > kplus;
   std::vector< std::pair<double,int> > kminus;
   std::vector< std::pair<double,int> > k0s;
   std::vector< std::pair<double,int> > lambda;
   std::vector< std::pair<double,int> > proton;
   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "piplus", model, NDF, Versions[iv] );
      piplus.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "piminus", model, NDF, Versions[iv] );
      piminus.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "kplus", model, NDF, Versions[iv] );
      kplus.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "kminus", model, NDF, Versions[iv] );
      kminus.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "k0s", model, NDF, Versions[iv] );
      k0s.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "lambda", model, NDF, Versions[iv] );
      lambda.push_back( std::pair<double,int>(chi2,NDF) );
   }   
   for ( int iv=0; iv<NVersions; ++iv )
   {
      int NDF = 0;
      double chi2 = Chi2NA61IntegralData2015( beam, target, "proton", model, NDF, Versions[iv] );
      proton.push_back( std::pair<double,int>(chi2,NDF) );
   }   

   std::ofstream out_table;
   std::string table_name = model +"_" + beam + "_" + target + "_NA61.txt"; 
   
   std::cout << " Opening ascii file: " << table_name << std::endl;

   out_table.open( table_name.c_str() );
     
   out_table << std::right << std::setw(30) << model << ":  31GeV/c " << beam << " on " << target << " --> hadrons" << std::endl;
   out_table << std::right << std::setw(62) << " chi2/NDF calculated vs NA61 data " << std::endl;
   out_table << " " << std::endl;

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
      out_table << std::right << setw(12) << piplus[iv].first/piplus[iv].second ;
   }   
   out_table << "\n pi-       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << piminus[iv].first/piminus[iv].second ;
   }   
   out_table << "\n K+        "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << kplus[iv].first/kplus[iv].second ;
   }   
   out_table << "\n K-        "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << kminus[iv].first/kminus[iv].second ;
   }   
   out_table << "\n K0s       "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << k0s[iv].first/k0s[iv].second ;
   }   
   out_table << "\n Lambda    "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << lambda[iv].first/lambda[iv].second ;
   }   
   out_table << "\n proton    "; 
   for ( int iv=0; iv<NVersions; iv++ )
   {
      out_table << std::right << setw(12) << proton[iv].first/proton[iv].second ;
   }   

   out_table.close();
   
   return;

}
