#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>

#include <math.h>
#include <vector>

#include "Rtypes.h"
#include "TROOT.h"
#include "TRint.h"
#include "TObject.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"

#include "NA49.h"
#include "ReadNA49Data.C"
#include "Chi2Calc.C"
#include "Chi2CalcNA49.C"
#include "DrawNA49Spectra.C"

// #include "../test23/shared-root-macros/REGRESSION_TEST.h"

#ifndef G4VAL_PLOTNA49REGRE
#define G4VAL_PLOTNA49REGRE

void plot_dNdxF_pT_Regre( std::string beam, std::string target, std::string secondary, std::string model )
{

   /*
    Here should be special provision for neutrons because only dNdxF data are available
    */

   if ( secondary == "neutron" )
   {
   /*
    Here is special provision for neutrons because only dNdxF data are available
    */
      TCanvas* myc = new TCanvas( "myc", "", 600, 600 );
      TPad* pad1 = new TPad( "pad1", "", 0.01, 0.12, 0.99, 0.99 );
      pad1->Draw();
      pad1->Divide(1.,2.,0.,0.);
      pad1->cd(1); gPad->SetRightMargin(0.025);
      drawIntegratedSpectrumRegre( beam, target, secondary, "dNdxF", model ); 
      pad1->cd(2); gPad->SetRightMargin(0.025);
      gPad->SetLogy();
      drawIntSpectrumMC2DataRegre( beam, target, secondary, "dNdxF", model, false );
      myc->cd();
      for ( int m=0; m<NVersions; ++m )
      {
         int NDF1=0;
         double chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "dNdxF", model, NDF1, Versions[m] );  
         std::ostringstream os1;
         os1 << (chi2/NDF1);
         std::string txt1 = " #chi^{2}/NDF = " + os1.str();
         txt1 += ( " for " + Versions[m] + " vs NA49 Data" );
         TLatex* ltxt1 = new TLatex( 0.10, 0.10-m*0.025, txt1.c_str() );
         ltxt1->SetTextSize(0.025);
         ltxt1->Draw();  
      } 
      myc->cd();
      std::string output = "proton-C-158GeV-neutron-" + model + "-regre.gif";
      myc->Print( output.c_str() );
      return;      
   }
   
   
   
   TCanvas* myc   = new TCanvas("myc","",1000,600);

   TPad* pad1 = new TPad( "pad1", "", 0.01, 0.12, 0.49, 0.99 );
   
   pad1->Draw();
   pad1->Divide(1.,2.,0.,0.);
   pad1->cd(1); gPad->SetRightMargin(0.025);
   drawIntegratedSpectrumRegre( beam, target, secondary, "dNdxF", model ); 
   pad1->cd(2); gPad->SetRightMargin(0.025);
   gPad->SetLogy();
   if ( secondary == "proton" || secondary == "antiproton" )
   {
     drawIntSpectrumMC2DataRegre( beam, target, secondary, "dNdxF", model, false ); // no legent for mc/data for (anti)protons
   }
   else
   {
      drawIntSpectrumMC2DataRegre( beam, target, secondary, "dNdxF", model );
   }

   myc->cd();
      
   TPad* pad2 = new TPad( "pad2", "", 0.51, 0.12, 0.99, 0.99 );

   pad2->Draw();
   pad2->Divide(1.,2.,0.,0.);
   pad2->cd(1); gPad->SetRightMargin(0.025);
   drawIntegratedSpectrumRegre( beam, target, secondary, "pT", model, false ); //, false ); // no legend because it overlaps with the contents
   pad2->cd(2); gPad->SetRightMargin(0.025);
   gPad->SetLogy();
   drawIntSpectrumMC2DataRegre( beam, target, secondary, "pT", model );
   
   myc->cd();
   for ( int m=0; m<NVersions; ++m )
   {
      int NDF1=0;
      int NDF2 = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "dNdxF", model, NDF1, Versions[m] );  
      std::ostringstream os1;
      os1 << (chi2/NDF1);
      std::string txt1 = " #chi^{2}/NDF = " + os1.str();
      txt1 += ( " for " + Versions[m] + " vs NA49 Data" );
      TLatex* ltxt1 = new TLatex( 0.05, 0.10-m*0.025, txt1.c_str() );
      ltxt1->SetTextSize(0.025);
      ltxt1->Draw();  
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "pT", model, NDF2, Versions[m] );
      std::ostringstream os2;
      os2 << (chi2/NDF2);
      std::string txt2 = " #chi^{2}/NDF = " + os2.str();
      txt2 += ( " for " + Versions[m] + " vs NA49 Data" );
      TLatex* ltxt2 = new TLatex( 0.55, 0.10-m*0.025, txt2.c_str() );
      ltxt2->SetTextSize(0.025);
      ltxt2->Draw();
   }

   myc->cd();
   
   std::string output = beam + "-" + target + "-158GeV-" + secondary + "-" + model + "-regre.gif";
   myc->Print( output.c_str() );
   

   return;

} 

#endif
