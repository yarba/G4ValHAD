#include <iostream>
#include <fstream>
#include <sstream>
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

#include "HARP.h"
#include "ReadHARPData.C"
#include "DrawHARPSpectra.C"

#include "REGRESSION_TEST.h"
#include "Chi2Calc.C"

#ifndef G4VAL_PLOTHARPREGRE_C
#define G4VAL_PLOTHARPREGRE_C

void PlotHARPAnalysisRegre( std::string beam, std::string target, std::string energy,
                       std::string secondary,
		       std::string region, 
		       std::string model )
{

   ReadHARPData( beam, target, energy, secondary, region );
   
   TCanvas* myc  = 0;
   TPad*    pad = 0;
   
   if ( region == "FW" )
   {   
      myc = new TCanvas( "myc", "", 800, 800 );
      myc->cd();
      // myc->Divide( 2, 2 );
      pad = new TPad("pad", "", 0.01, 0.13, 0.99, 0.99 );
      myc->cd();
      pad->Draw();
      pad->Divide(2.,2.,0.,0.);
   }
   else if ( region == "LA" )
   {
      myc = new TCanvas( "myc", "", 1100, 1100 );
      // myc->Divide( 3, 3 );
      pad = new TPad("pad", "", 0.01, 0.13, 0.99, 0.99 );
      myc->cd();
      pad->Draw();
      pad->Divide(3.,3.,0.,0.);
   }

   for ( int i=0; i<NSetsHARP; i++ )
   {
      // myc->cd(i+1);
      myc->cd();
      pad->cd(i+1);
      gPad->SetRightMargin(0.1);
      PlotHARPHistoRegre( beam, target, energy, secondary, region, i, model, false );
   }
   
   TLegend* leg = new TLegend(0.7,0.01,0.99,0.12);
   TLegendEntry* entry = 0;
   for ( int m=0; m<NVersions; ++m )
   {
      entry = leg->AddEntry( "", Versions[m].c_str(),"L" );
      entry->SetLineColor( ColorVersion[m] );
      entry->SetLineWidth(3);
      entry->SetTextFont(62);
   }
   entry = leg->AddEntry("","exp.data","p");
   entry->SetMarkerStyle(22);
   entry->SetMarkerColor(kBlack /* kBlue */ );
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(62);
   leg->SetFillColor(kWhite);
   myc->cd();
   leg->Draw();
   
   std::string general = model + " vs HARP Data; #chi^{2}/NDF calculated over ";
   general += ( region + " theta bins" );

   TLatex* ltxt = new TLatex(0.1, 0.10, general.c_str() );
   ltxt->SetTextSize(0.02);
   myc->cd();
   ltxt->Draw();
   
   std::cout << " Geant4 model/physlis = " << model << std::endl;
   
   for ( int m=0; m<NVersions; ++m )
   {

      double chi2 = 0.;
      int NDF = 0;
      std::ostringstream os;

      chi2 = Chi2MomSpectrumHARP( beam, target, energy, secondary, region, model, NDF, Versions[m] );

      os << (chi2/NDF);
      std::string txt1 = "#chi^{2}/NDF = " + os.str();
      txt1 += ( " for " + Versions[m] );
      TLatex* ltxt1 = new TLatex( 0.1, 0.08-0.02*m, txt1.c_str() );
      ltxt1->SetTextSize(0.02);
      myc->cd();
      ltxt1->Draw();
      
      std::cout << " chi2/NDF = " << chi2 << "/" << NDF << " = " << chi2/NDF << " for " << Versions[m] << std::endl;

   }

   std::string output = beam + "-" + target + "-" +  energy + "GeV-" + secondary + "-" + region;
   output += ( "-" + model + "-regre.gif" );
   myc->Print( output.c_str() );
   
   return;

}

void PlotHARPRegreForReview()
{

   TCanvas* myc = new TCanvas("myc","",800,500);
   
   ReadHARPData( "proton","Ta","8.0","piminus","LA");
   myc->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   
   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", "piminus", 0.1, 0.15, "Shielding" );
//   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", "piminus", 0.1, 0.15, "ftfp" );

//   std::string txt = "Phys.list: Shielding";
   std::string txt = "Model: FTFP";
   TLatex* ltxt = new TLatex( 0.25, 3000, txt.c_str() );
   ltxt->SetTextSize(0.05);
   myc->cd();
   ltxt->Draw();

   return;
      
   myc->Print("proton-Ta-8.0GeV-piminus-Shielding-regre.gif");
   
   TCanvas* myc1 = new TCanvas("myc1","",800,500);
   
//    ReadHARPData( "proton","Ta","8.0", "piminus", "LA");
   myc1->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   
   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", "piminus", 0.1, 0.15, "ShieldingM" );

   std::string txt1 = "Phys.list: ShieldingM";
   TLatex* ltxt1 = new TLatex( 0.25, 3000, txt1.c_str() );
   ltxt1->SetTextSize(0.05);
   myc1->cd();
   ltxt1->Draw();

   myc1->Print("proton-Ta-8.0GeV-piminus-ShieldingM-regre.gif");

   return;
   
}

// void PlotHARPRegreForMu2e( std::string secondary, std::string model )
void PlotHARPRegreForMu2e( std::string secondary, std::string tg, std::string model )
{

   TCanvas* myc = new TCanvas( "myc", "", 800, 800 );
// -->   ReadHARPData( "proton", "Ta", "8.0", secondary, "LA" );
   ReadHARPData( "proton", tg, "8.0", secondary, "LA" );
   
// -->   std::string txt1 = "8.0GeV/c proton + Ta #rightarrow " + secondary;
   std::string txt1 = model + ":   8.0GeV/c proton + " + tg + " #rightarrow " + secondary;
   TLatex* ltxt1 = new TLatex( 0.25, 0.97, txt1.c_str() );
   ltxt1->SetTextSize(0.025);
   myc->cd();
   ltxt1->Draw();
   
   double ChiSqTotal[NVersions]; 
   int NDFTotal[NVersions];
   for ( int m=0; m<NVersions; ++m )
   {
      ChiSqTotal[m] = 0.;
      NDFTotal[m] = 0.;
   }
    
// -->     TPad* mypad = new TPad( "mypad", "", 0.01, 0.135, 0.99, 0.925 );
   TPad* mypad = new TPad( "mypad", "", 0.01, 0.15, 0.99, 0.95 );
   mypad->Divide( 2., 2., 0.01, 0.01 ); 
   myc->cd();
   mypad->Draw();
   
   mypad->cd(1);
// -->   gPad->SetRightMargin(0.1);
// -->   gPad->SetLeftMargin(0.15);
// -->   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", secondary, 0.1, 0.15, model );
   PlotHARPThetaSpectrumRegre( "proton", tg, "8.0", secondary, 0.1, 0.15, model );
   for ( int m=0; m<NVersions; ++m )
   {
      ChiSqTotal[m] += ChiSqThetaRegre[m];
      NDFTotal[m] += NDFThetaRegre[m];
   }

   mypad->cd(2);
// -->   gPad->SetRightMargin(0.01);
// -->   gPad->SetLeftMargin(0.1);
// -->   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", secondary, 0.15, 0.2, model );
   PlotHARPThetaSpectrumRegre( "proton", tg, "8.0", secondary, 0.15, 0.2, model );
   for ( int m=0; m<NVersions; ++m )
   {
      ChiSqTotal[m] += ChiSqThetaRegre[m];
      NDFTotal[m] += NDFThetaRegre[m];
   }

   mypad->cd(3);
// -->   gPad->SetRightMargin(0.1);
// -->   gPad->SetLeftMargin(0.15);
// -->   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", secondary, 0.2, 0.25, model );
   PlotHARPThetaSpectrumRegre( "proton", tg, "8.0", secondary, 0.2, 0.25, model );
   for ( int m=0; m<NVersions; ++m )
   {
      ChiSqTotal[m] += ChiSqThetaRegre[m];
      NDFTotal[m] += NDFThetaRegre[m];
   }

   mypad->cd(4);
// -->   gPad->SetRightMargin(0.01);
// -->   gPad->SetLeftMargin(0.1);
// -->   PlotHARPThetaSpectrumRegre( "proton","Ta", "8.0", secondary, 0.25, 0.3, model );
   PlotHARPThetaSpectrumRegre( "proton", tg, "8.0", secondary, 0.25, 0.3, model );
   for ( int m=0; m<NVersions; ++m )
   {
      ChiSqTotal[m] += ChiSqThetaRegre[m];
      NDFTotal[m] += NDFThetaRegre[m];
      std::cout << " chi2/NDF = " << ChiSqTotal[m] << "/" << NDFTotal[m] << " = " << ChiSqTotal[m]/NDFTotal[m] << "  for " << Versions[m] << std::endl;
   }
   
   TLegend* leg = new TLegend(0.7,0.01,0.99,0.14);
   TLegendEntry* entry = 0;
   for ( int m=0; m<NVersions; ++m )
   {
      entry = leg->AddEntry( "", Versions[m].c_str(),"L" );
      entry->SetLineColor( ColorVersion[m] );
      entry->SetLineWidth(3);
      entry->SetTextFont(62);
   }
   entry = leg->AddEntry("","exp.data","p");
   entry->SetMarkerStyle(22);
   entry->SetMarkerColor(kBlack /* kBlue */ );
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(62);
   leg->SetFillColor(kWhite);
   myc->cd();
   leg->Draw();

   // --> TLatex* genltxt = new TLatex( 0.1, 0.10, "#chi^{2}/NDF is calculated for the given P(pi-) bins" );
   TLatex* genltxt = new TLatex( 0.1, 0.13, "#chi^{2}/NDF is calculated for the given P(pi-) bins" );
   genltxt->SetTextFont(62);
   genltxt->SetTextSize(0.0225);
   myc->cd();
   genltxt->Draw();
   TLatex* chi2ltxt[NVersions];
   for ( int m=0; m<NVersions; ++m )
   {
      std::ostringstream os1;
      std::ostringstream os2;
      os1 << ChiSqTotal[m];
      os2 << ChiSqTotal[m]/NDFTotal[m];
      std::string chi2txt = "#chi^{2}/NDF = " + os1.str() + "/" + std::to_string(NDFTotal[m]) + " = " + os2.str() + " for " + Versions[m];
      // --> chi2ltxt[m] = new TLatex( 0.1, (0.075-0.025*m), chi2txt.c_str() );
      chi2ltxt[m] = new TLatex( 0.1, (0.105-0.025*m), chi2txt.c_str() );
      chi2ltxt[m]->SetTextFont(62);
      chi2ltxt[m]->SetTextSize(0.0225);
      myc->cd();
      chi2ltxt[m]->Draw();       
   }

// -->   std::string out = "proton-Ta-8.0GeV-" + secondary + "-theta-spectra-" + model;
   std::string out = "proton-" + tg + "-8.0GeV-" + secondary + "-theta-spectra-" + model;
   out += "-regre.gif";
   myc->Print( out.c_str() );


   return;

}


#endif
