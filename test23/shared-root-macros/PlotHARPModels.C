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

#include "Chi2Calc.C"
#include "Chi2CalcHARP.C"

#ifndef G4VAL_PLOTHARPMODELS_C
#define G4VAL_PLOTHARPMODELS_C

void PlotHARPAnalysis( std::string beam, std::string target, std::string energy,
                       std::string secondary,
		       std::string region )
{

   ReadHARPData( beam, target, energy, secondary, region );
   
   TCanvas* myc = 0;
   TPad*    pad = 0;
   
   if ( region == "FW" )
   {   
      myc = new TCanvas( "myc", "", 800, 800 );
      // myc->Divide( 2, 2 );   
      pad = new TPad( "pad", "", 0.01, 0.13, 0.99, 0.99 );
      myc->cd();
      pad->Draw();
      pad->Divide(2.,2.,0.,0.);
   }
   else if ( region == "LA" )
   {
      myc = new TCanvas( "myc", "", 1100, 1100 );
      // myc->Divide( 3, 3 );
      pad = new TPad( "pad", "", 0.01, 0.13, 0.99, 0.99 );
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
//      gPad->SetLeftMargin(0.1);
//      PlotHARPHisto( beam, target, energy, secondary, region, i, true, false, 0.05  );
      PlotHARPHisto( beam, target, energy, secondary, region, i, false );
   }
   
   TLegend* leg = new TLegend(0.7,0.01,0.99,0.12);
   TLegendEntry* entry = 0;
   for ( int m=0; m<NModels_IE; ++m )
   {
      entry = leg->AddEntry( "", ModelName_IE[m].c_str(),"L" );
      entry->SetLineColor( ColorModel_IE[m] );
      entry->SetLineWidth(3);
      entry->SetTextFont(62);
   }
   entry = leg->AddEntry("","exp.data","p");
   entry->SetMarkerStyle(22);
   entry->SetMarkerColor(kBlue);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(62);
   leg->SetFillColor(kWhite);
   myc->cd();
   leg->Draw();

   myc->cd();
   
   std::string general = "MC vs HARP Data; #chi^{2}/NDF calculated over ";
   general += ( region + " theta bins" );

   TLatex* ltxt = new TLatex(0.1, 0.10, general.c_str() );
   ltxt->SetTextSize(0.02);
   myc->cd();
   ltxt->Draw();
   
   for ( int m=0; m<NModels_IE; ++m )
   {

      double chi2 = 0.;
      int NDF = 0;
      std::ostringstream os;

      chi2 = Chi2MomSpectrumHARP( beam, target, energy, secondary, region, ModelName_IE[m], NDF );

      os << (chi2/NDF);
      std::string txt1 = "#chi^{2}/NDF = " + os.str();
      txt1 += ( " for " + ModelName_IE[m] );
      TLatex* ltxt1 = new TLatex( 0.1, 0.08-0.02*m, txt1.c_str() );
      ltxt1->SetTextSize(0.02);
      myc->cd();
      ltxt1->Draw();
      
      std::cout << " chi2/NDF = " << chi2 << "/" << NDF << " = " << chi2/NDF << " for model " << ModelName_IE[m] << std::endl;

   }

   std::string output = beam + "-" + target + "-" + energy + "GeV-" + secondary + "-";
   output += region;
   output += "-models.gif";
   myc->Print( output.c_str() );
   
   return;

}

void PlotHARPAnaMC2Data(std::string beam, std::string target, std::string energy,
                       std::string secondary,
		       std::string region )
{

   ReadHARPData( beam, target, energy, secondary, region );
   
   TCanvas* myc = 0;
   TPad*    pad = 0;
   
   if ( region == "FW" )
   {   
      myc = new TCanvas( "myc", "", 800, 800 );
      // myc->Divide( 2, 2 );
      pad = new TPad( "pad", "", 0.01, 0.11, 0.99, 0.99 );
      myc->cd();
      pad->Draw();
      pad->Divide(2.,2.,0.,0.);
   }
   else if ( region == "LA" )
   {
      myc = new TCanvas( "myc", "", 1200, 1200 );
      // myc->Divide( 3, 3 );
      pad = new TPad( "pad", "", 0.01, 0.11, 0.99, 0.99 );
      myc->cd();
      pad->Draw();
      pad->Divide(3.,3.,0.,0.);
   }

   for ( int i=0; i<NSetsHARP; i++ )
   {
      // myc->cd(i+1);
      myc->cd();
      pad->cd(i+1);
      gPad->SetRightMargin(0.01);
      PlotHARPMC2Data( beam, target, energy, secondary, region, i );
   }
   
   myc->cd();
   
   return;

}

void PlotHARPAnalysisCombined( std::string beam, std::string target, std::string energy,
                               std::string secondary,
		               std::string region )
{

   ReadHARPData( beam, target, energy, secondary, region );
   
   TCanvas* myc = 0;
   
   float delta = 0.;
   int NX = 1;
   int NY = 1;
   
   if ( region == "FW" )
   {   
      myc = new TCanvas( "myc", "", 800, 800 );
      delta = 0.48;
      NX = 2;
      NY = 2;      
      // myc->Divide( 2, 2 );
   }
   else if ( region == "LA" )
   {
      myc = new TCanvas( "myc", "", 1200, 1200 );
      // myc->Divide( 3, 3 );
      delta = 0.31;
      NX = 3;
      NY = 3;
   }

   int iset = 0;
   for ( int i=0; i<NX; ++i )
   {
      float y2 = 1. - 0.01*(i+1) - (delta+0.01)*i;
      float y1 = y2 - delta;
      for ( int j=0; j<NY; ++j )
      {
	 std::ostringstream osCount;
	 osCount << iset;
         float x1 = 0.01 + delta*j + 0.02*j;
         float x2 = x1 + delta;
//         std::cout << " x1= " << x1 << " x2= " << x2 << " y1= " << y1 << " y2= " << y2 << std::endl;	 
//         std::ostringstream osCount;
//         osCount << iset;
         std::string pname = "pad" + osCount.str();
	 TPad* pad = new TPad("pname","",x1,y1,x2,y2);
         myc->cd();
	 pad->Draw();
         pad->Divide(1.,2.,0.,0.);
         pad->cd(1); gPad->SetRightMargin(0.025);
//         PlotHARPHisto( beam, target, energy, secondary, region, iset, true, true, 0.1 );
         PlotHARPHisto( beam, target, energy, secondary, region, iset, true, false, 0.62 );
         pad->cd(2); gPad->SetRightMargin(0.025);
	 gPad->SetLogy();
         PlotHARPMC2Data( beam, target, energy, secondary, region, iset, false, false );
	 iset++;
      }   
   }
   
   
   
/*
   for ( int i=0; i<NSets; i++ )
   {
//      myc->cd(i+1);
      std::ostringstream osCount;
      osCount << i;
      std::string pname = "pad" + osCount.str();
      float x1 = 0.01*(i+1) + delta*i;
      float x2 = x1 + delta + 0.01; 
      
      std::cout << " x1= " << x1 << " x2= " << x2 << " y1= " << y1 << " y2= " << y2 << std::endl;
      
      TPad* pad = new TPad("pname","",x1,y1,x2,y2);
      pad->Draw();
      pad->Divide(1.,2.,0.,0.);
      pad->cd(1); gPad->SetRightMargin(0.025);
      PlotHARPMC2Data( beam, target, energy, secondary, region, i );
      pad->cd(2); gPad->SetRightMargin(0.025);
      PlotHARPMC2Data( beam, target, energy, secondary, region, i );

   }
*/   

   myc->cd();
   
   std::string output = beam + "-" + target + "-" + energy + "GeV-" + secondary + "-";
   output += region;
   output += "-models.gif";
   myc->Print( output.c_str() );
   
   return;

}

void PlotHARPForTalk()
{

   // selected plots:
   // 5GeV pi+ on C --> pi+   0.15<theta<0.2 && 0.95<theta<1.15 
   // 5GeV p   on C --> pi+   0.15<theta<0.2 && 0.95<theta<1.15
   // i.e. FW inib=2 && LA ibin=3
   

   TCanvas* myc = new TCanvas("myc","",800,800);

   
   TPad* pad1 = new TPad( "pad1", "", 0.0, 0.15, 0.485, 0.99 );
   TPad* pad2 = new TPad( "pad2", "", 0.515, 0.15, 1.00, 0.99 );
   
   ReadHARPData( "piplus","C","5.0", "piplus", "FW");
   myc->cd();
   pad1->Draw();
   pad1->Divide(1.,2.,0.,0.);
   pad1->cd(1);
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   PlotHARPHisto( "piplus", "C", "5.0", "piplus", "FW", 2, true );
   pad1->cd(2); 
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   gPad->SetLogy();
   PlotHARPMC2Data( "piplus", "C", "5.0", "piplus", "FW", 2, false, false );

   for ( int m=0; m<NModels_IE; ++m )
   {   
      double chi2 = 0;
      int    NDF  = 0;   
      std::string histofile = "./harp-histo/piplusC5.0GeV" + ModelName_IE[m] + ".root" ; 
      TFile* f = new TFile( histofile.c_str() );
      TH1F* h = (TH1F*)f->Get( "piplus_FW_2" );
      TGraphErrors* gdata = getHARPAsGraph( 2 );
      chi2 += Chi2( gdata, h, NDF );
      std::cout << " chi2/NDF = " << chi2 << "/" << NDF << " = " << (chi2/NDF) << " for " << ModelName_IE[m] << std::endl;
      std::ostringstream os;
      os.precision(3);
      os << (chi2/NDF);
      std::string txt = " #chi^{2}/NDF = " + os.str();
      txt += ( " for " + ModelName_IE[m] );
      TLatex* ltxt = new TLatex( 0.1, 0.12-m*0.025, txt.c_str() );
      ltxt->SetTextSize(0.025);
      myc->cd();
      ltxt->Draw();        
   }
   
   ReadHARPData( "piplus","C","5.0", "piplus", "LA");
   myc->cd();
   pad2->Draw();
   pad2->Divide(1.,2.,0.,0.);
   pad2->cd(1);
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   PlotHARPHisto( "piplus", "C", "5.0", "piplus", "LA", 3, true );
   pad2->cd(2); 
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   gPad->SetLogy();
   PlotHARPMC2Data( "piplus", "C", "5.0", "piplus", "LA", 3, false, false );
   for ( int m=0; m<NModels_IE; ++m )
   {   
      double chi2 = 0;
      int    NDF  = 0;   
      std::string histofile = "./harp-histo/piplusC5.0GeV" + ModelName_IE[m] + ".root" ; 
      TFile* f = new TFile( histofile.c_str() );
      TH1F* h = (TH1F*)f->Get( "piplus_LA_3" );
      TGraphErrors* gdata = getHARPAsGraph( 3 );
      chi2 += Chi2( gdata, h, NDF );
      std::cout << " chi2/NDF = " << chi2 << "/" << NDF << " = " << (chi2/NDF) << " for " << ModelName_IE[m] << std::endl;
      std::ostringstream os;
      os.precision(3);
      os << (chi2/NDF);
      std::string txt = " #chi^{2}/NDF = " + os.str();
      txt += ( " for " + ModelName_IE[m] );
      TLatex* ltxt = new TLatex( 0.6, 0.12-m*0.025, txt.c_str() );
      ltxt->SetTextSize(0.025);
      myc->cd();
      ltxt->Draw();        
   }
       
   return;

}

void PlotHARPForReview()
{

   TCanvas* myc = new TCanvas("myc","",800,500);
   
   ReadHARPData( "proton","Ta","8.0", "piminus", "LA");
   myc->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   
   PlotHARPThetaSpectrum( "proton","Ta", "8.0", "piminus", 0.1, 0.15 );
   
   myc->Print("proton-Ta-8.0GeV-piminus-models.gif");
          
   return;

}

void PlotHARPForMu2e( std::string secondary )
{

   TCanvas* myc = new TCanvas( "myc", "", 800, 800 );
   ReadHARPData( "proton", "Ta", "8.0", secondary, "LA" );
   
   std::string txt1 = "8.0GeV/c proton + Ta #rightarrow " + secondary;
   TLatex* ltxt1 = new TLatex( 0.35, 0.97, txt1.c_str() );
   ltxt1->SetTextSize(0.025);

   // myc->cd(1);
   TPad* pad1 = new TPad("pad1", "", 0.01, 0.46, 0.49, 0.9 );
   myc->cd();
   pad1->Draw();
   pad1->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   
   std::cout << " about to plot on pad1 " << std::endl;
   
   PlotHARPThetaSpectrum( "proton","Ta", "8.0", secondary, 0.1, 0.15 );

   // myc->cd(2);
   TPad* pad2 = new TPad("pad2", "", 0.51, 0.46, 0.99, 0.9 );
   myc->cd();
   pad2->Draw();
   pad2->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   PlotHARPThetaSpectrum( "proton","Ta", "8.0", secondary, 0.15, 0.2 );

   // myc->cd(3);
   TPad* pad3 = new TPad("pad3", "", 0.01, 0.01, 0.49, 0.45 );
   myc->cd();
   pad3->Draw();
   pad3->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   PlotHARPThetaSpectrum( "proton","Ta", "8.0", secondary, 0.2, 0.25 );

   // myc->cd(4);
   TPad* pad4 = new TPad("pad4", "", 0.51, 0.01, 0.99, 0.45 );
   myc->cd();
   pad4->Draw();
   pad4->cd();
   gPad->SetRightMargin(0.01);
   gPad->SetLeftMargin(0.15);
   PlotHARPThetaSpectrum( "proton","Ta", "8.0", secondary, 0.25, 0.3 );

   std::string out = "proton-Ta-8.0GeV-" + secondary + "-theta-spectra-models.gif";
   myc->Print( out.c_str() );

   return;

}

#endif
