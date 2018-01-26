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

// #include "../test19/G4MODELS_HighEnergy.h"

#ifndef G4VAL_PLOTNA49MODELS_C
#define G4VAL_PLOTNA49MODELS_C

void plot_dNdxF_pT( std::string beam, std::string target, std::string secondary )
{

   if ( secondary == "neutron" )
   {
   /*
    Here is special provision for neutrons because only dNdxF data are available
    */
      TCanvas* myc = new TCanvas( "myc", "", 600, 600 );
      TPad* pad1 = new TPad( "pad1", "", 0.01, 0.14, 0.99, 0.99 );
      pad1->Draw();
      pad1->Divide(1.,2.,0.,0.);
      pad1->cd(1); gPad->SetRightMargin(0.025);
      drawIntegratedSpectrum( beam, target, secondary, "dNdxF" ); 
      pad1->cd(2); gPad->SetRightMargin(0.025);
      gPad->SetLogy();
      drawIntSpectrumMC2Data( beam, target, secondary, "dNdxF" );
      myc->cd();
      TPad* pad3 = new TPad("pad3","",0.01, 0.01, 0.99, 0.125);
      pad3->Draw();
      for ( int m=0; m<NModels_HE; ++m )
      {
         int NDF1=0;
         double chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "dNdxF", ModelName_HE[m], NDF1 );  
         std::ostringstream os1;
	 if ( (chi2/NDF1) < 100 )
	 {
	    os1.precision(3);
	 }
	 else
	 {
	    os1.precision(4);
	 }
         os1 << (chi2/NDF1);
         std::string txt1 = " #chi^{2}/NDF = " + os1.str();
         txt1 += ( " for " + ModelName_HE[m] ); // + " vs NA49 Data" );
         TLatex* ltxt1 = new TLatex( 0.1, 0.8-m*0.25, txt1.c_str() );
         ltxt1->SetTextSize(0.25);
         pad3->cd();
         ltxt1->Draw();  
      } 
      myc->cd();
      myc->Print( "proton-C-158GeV-neutron-models.gif" );
      return;      
   }
      
   TCanvas* myc   = new TCanvas("myc","",1000,600);

   TPad* pad1 = new TPad( "pad1", "", 0.01, 0.14, 0.49, 0.99 );
   
   pad1->Draw();
   pad1->Divide(1.,2.,0.,0.);
   pad1->cd(1); gPad->SetRightMargin(0.025);
   drawIntegratedSpectrum( beam, target, secondary, "dNdxF" ); 
   pad1->cd(2); gPad->SetRightMargin(0.025);
   gPad->SetLogy();
   if ( secondary == "proton" || secondary == "antiproton" )
   {
     drawIntSpectrumMC2Data( beam, target, secondary, "dNdxF", false ); // no legent for mc/data for (anti)protons
   }
   else
   {
      drawIntSpectrumMC2Data( beam, target, secondary, "dNdxF" );
   }

   myc->cd();
      
   TPad* pad2 = new TPad( "pad2", "", 0.51, 0.14, 0.99, 0.99 );

   pad2->Draw();
   pad2->Divide(1.,2.,0.,0.);
   pad2->cd(1); gPad->SetRightMargin(0.025);
   drawIntegratedSpectrum( beam, target, secondary, "pT", false ); // no legend because it overlaps with the contents
   pad2->cd(2); gPad->SetRightMargin(0.025);
   gPad->SetLogy();
   drawIntSpectrumMC2Data( beam, target, secondary, "pT" );
   
   myc->cd();

   TPad* pad3 = new TPad("pad3","",0.01, 0.01, 0.49, 0.125);
   pad3->Draw();
   // pad3->cd();
   myc->cd();
   TPad* pad4 = new TPad("pad4","",0.51, 0.01, 0.99, 0.125);
   pad4->Draw();
   // pad4->cd();
   for ( int m=0; m<NModels_HE; ++m )
   {
      int NDF1=0;
      int NDF2 = 0;
      double chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "dNdxF", ModelName_HE[m], NDF1 );  
      std::ostringstream os1;
      if ( (chi2/NDF1) < 100 )
      {
         os1.precision(3);
      }
      else
      {
         os1.precision(4);
      }
      os1 << (chi2/NDF1);
      std::string txt1 = " #chi^{2}/NDF = " + os1.str();
      txt1 += ( " for " + ModelName_HE[m] ); // + " vs NA49 Data" );
      TLatex* ltxt1 = new TLatex( 0.1, 0.8-m*0.25, txt1.c_str() );
      ltxt1->SetTextSize(0.25);
      pad3->cd();
      ltxt1->Draw();  
      chi2 = Chi2IntegratedSpectrumNA49( beam, target, secondary, "pT", ModelName_HE[m], NDF2 );
      std::ostringstream os2;
      if ( (chi2/NDF2) < 100 )
      {
         os2.precision(3);
      }
      else
      {
         os2.precision(4);
      }
      os2 << (chi2/NDF2);
      std::string txt2 = " #chi^{2}/NDF = " + os2.str();
      txt2 += ( " for " + ModelName_HE[m] ); // + " vs NA49 Data" );
      TLatex* ltxt2 = new TLatex( 0.1, 0.80-m*0.25, txt2.c_str() );
      ltxt2->SetTextSize(0.25);
      pad4->cd();
      ltxt2->Draw();
   }

   myc->cd();
   
   std::string output = beam + "-" + target + "-158GeV-" + secondary;
   output += "-models.gif";
   
   myc->Print( output.c_str() );

   return;

} 

void plotDDiffForTalk()
{

// this is for NBI-Sim talk on Sept.22 , 2014 && K.G.'s talk at the G4-2014 in OKinawa

   readDDiffSpectra( "proton", "C", "piplus" );
   
   TCanvas* myc = new TCanvas("myc","",800, 600); 
   
//   myc->Divide(2,1);

   TPad* pad1 = new TPad("pad1", "", 0.0, 0.15, 0.485, 0.99 ); 
   TPad* pad2 = new TPad("pad2", "", 0.515, 0.15, 1.00, 0.99); 
   
   // myc->cd(1);
   myc->cd();
   pad1->Draw();
   pad1->cd();
   gPad->SetLogy();
   draw1DDiffXSec( "proton","C", "piplus", 8 );
   
   // myc->cd(2);
   myc->cd();
   pad2->Draw();
   pad2->cd();
   gPad->SetLogy();
   draw1DDiffXSec( "proton","C", "piplus", 16 );
   
   double chi2 =0.;
   int    NDF  =0;
   for ( int m=0; m<NModels_HE; m++ )
   {
      std::string histofile = "./na49-histo/protonC158.0GeV" + ModelName_HE[m] + ".root"; 
      TFile* f = new TFile( histofile.c_str() );
      chi2 = 0.;
      NDF  =0;
      TH1F* h1 = (TH1F*)f->Get( "pTpip15" );
      h1->Scale(226.);
      TGraphErrors* gdata1 = get1DDiffXSecAsGraph( 8 ); // for pi+, the count is offset by 7
      chi2 = Chi2( gdata1, h1, NDF );
      std::cout << "chi2/ndf = " << chi2 << "/" << NDF << " = " << (chi2/NDF) << " for " << ModelName_HE[m] << std::endl; 
      std::ostringstream os1;
      os1.precision(3);
      os1 << (chi2/NDF);
      std::string txt1 = " #chi^{2}/NDF = " + os1.str();
      txt1 += ( " for " + ModelName_HE[m] );
      TLatex* ltxt1 = new TLatex( 0.05, 0.12-m*0.028, txt1.c_str() );
      ltxt1->SetTextSize(0.028);
      myc->cd();
      ltxt1->Draw();     
      chi2 = 0.;
      NDF  =0;
      TH1F* h2 = (TH1F*)f->Get( "pTpip23" );
      h2->Scale(226.);
      TGraphErrors* gdata2 = get1DDiffXSecAsGraph( 15 ); // for pi+, the count is offset by 7
      chi2 = Chi2( gdata2, h2, NDF );
      std::cout << "chi2/ndf = " << chi2 << "/" << NDF << " = " << (chi2/NDF) << " for " << ModelName_HE[m] << std::endl; 
      std::ostringstream os2;
      os2.precision(3);
      os2 << (chi2/NDF);
      std::string txt2 = " #chi^{2}/NDF = " + os2.str();
      txt2 += ( " for " + ModelName_HE[m] );
      TLatex* ltxt2 = new TLatex( 0.55, 0.12-m*0.028, txt2.c_str() );
      ltxt2->SetTextSize(0.028);
      myc->cd();
      ltxt2->Draw();     
   }
   
   
   return;

}

#endif
