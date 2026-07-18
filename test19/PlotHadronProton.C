#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <iomanip>
#include <string>
//#include <list>

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

int NModels = 2;
std::string Model[2] = { "bertini11.2", "bertini" };
int Color[2] = { kBlue, kRed }; 

void PlotHadronProton( std::string beam, std::string mom, std::string sec )
{

   TCanvas* cnv_fw = new TCanvas( "cnv_fw", "", 700, 700 );
   TPad*    pad_fw = new TPad( "pad_fw", "", 0.01, 0.13, 0.99, 0.99 );
   cnv_fw->cd();
   pad_fw->Draw();
   pad_fw->Divide( 2., 2., 0.005, 0.005 ); 
   
   TCanvas* cnv_la = new TCanvas( "cnv_la", "", 960, 960 );
   TPad*    pad_la = new TPad( "pad_la", "", 0.01, 0.13, 0.99, 0.99 );
   cnv_la->cd();
   pad_la->Draw();
   pad_la->Divide( 3., 3., 0.005, 0.005 ); 
   
   TLegend* leg = new TLegend(0.7,0.01,0.99,0.12);
   TLegendEntry* entry = 0;
   for ( int im=0; im<NModels; ++im )
   {
      entry = leg->AddEntry( "", Model[im].c_str(), "L" );
      entry->SetLineColor( Color[im] );
      entry->SetLineWidth(2);
      entry->SetTextFont(62);
   }
   leg->SetFillColor(kWhite);
   //leg->SetBorderSize(0);

   // std::vector<TFile*> hfiles;
   TFile* hfiles[NModels];  
   
   for ( int im=0; im<NModels; ++im )
   {
      std::string hfname = "./harp-histo/";
      hfname += beam + "H" + mom + "GeV" + Model[im] + ".root";
      // hfiles.push_back(TFile::Open(hfname.c_str()));
      hfiles[im] = TFile::Open(hfname.c_str());
   }
   
   TH1D* hi[NModels];
   
   int imax = 0;
   double ymax = -1.;
   
   for ( int ih=0; ih<4; ++ih )
   {
      std::string hname = sec + "_FW_" + std::to_string(ih);
      pad_fw->cd(ih+1);
      ymax = -1.;
      for ( int im=0; im<NModels; ++im )
      {
         hi[im] = (TH1D*)hfiles[im]->Get(hname.c_str());
	 hi[im]->SetStats(0);
	 hi[im]->SetLineColor( Color[im] );
	 hi[im]->SetLineWidth(2);
	 hi[im]->GetXaxis()->SetTitle("momentum (GeV/c)");
	 hi[im]->GetXaxis()->SetLabelFont(62);
	 hi[im]->GetXaxis()->SetTitleFont(62);
	 hi[im]->GetXaxis()->CenterTitle();
	 hi[im]->GetYaxis()->SetTitle("d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	 hi[im]->GetYaxis()->SetLabelFont(62);
	 hi[im]->GetYaxis()->SetTitleFont(62);
	 hi[im]->GetYaxis()->CenterTitle();
	 imax = hi[im]->GetMaximumBin();
         if ( ymax < (hi[im]->GetBinContent(imax)+2.*hi[im]->GetBinError(imax)) )
            ymax = hi[im]->GetBinContent(imax)+2.*hi[im]->GetBinError(imax);
         hi[im]->GetYaxis()->SetRangeUser( 0., ymax );	 
	 for ( int im1=0; im1<im; ++im1 )
	 {
	    hi[im1]->GetYaxis()->SetRangeUser( 0., ymax );	 
	 }
	 if ( im == 0 )
	 {
	    hi[im]->Draw("histE1");
	 }
	 else
	 {
	    hi[im]->Draw("histE1same");
	 }
	 gPad->Update();
	 pad_fw->Update();
	 cnv_fw->Update();
      }  
   }
   
   cnv_fw->cd();
   leg->Draw();
   
   std::string output_base = beam + "-H-" + mom + "GeV-" + sec ;
   
   std::string output = output_base + "-FW-bertini.gif";
   cnv_fw->Print(output.c_str());
      
   imax = 0;
   ymax = -1.;
   
   for ( int ih=0; ih<9; ++ih )
   {
      std::string hname = sec + "_LA_" + std::to_string(ih);
      pad_la->cd(ih+1);
      ymax = -1.;
      for ( int im=0; im<NModels; ++im )
      {
         hi[im] = (TH1D*)hfiles[im]->Get(hname.c_str());
	 hi[im]->SetStats(0);
	 hi[im]->SetLineColor( Color[im] );
	 hi[im]->SetLineWidth(2);
	 hi[im]->GetXaxis()->SetTitle("momentum (GeV/c)");
	 hi[im]->GetXaxis()->SetLabelFont(62);
	 hi[im]->GetXaxis()->SetTitleFont(62);
	 hi[im]->GetXaxis()->CenterTitle();
	 hi[im]->GetYaxis()->SetTitle("d^{2}#sigma / dpd#Theta [mb/(GeV/c/rad)]");
	 hi[im]->GetYaxis()->SetLabelFont(62);
	 hi[im]->GetYaxis()->SetTitleFont(62);
	 hi[im]->GetYaxis()->CenterTitle();
	 imax = hi[im]->GetMaximumBin();
         if ( ymax < (hi[im]->GetBinContent(imax)+2.*hi[im]->GetBinError(imax)) )
            ymax = hi[im]->GetBinContent(imax)+2.*hi[im]->GetBinError(imax);
	 hi[im]->GetYaxis()->SetRangeUser( 0., ymax );	 
	 for ( int im1=0; im1<im; ++im1 )
	 {
	    hi[im1]->GetYaxis()->SetRangeUser( 0., ymax );	 
	 }
	 if ( im == 0 )
	 {
	    hi[im]->Draw("histE1");
	 }
	 else
	 {
	    hi[im]->Draw("histE1same");
	 }
	 gPad->Update();
	 pad_la->Update();
	 cnv_la->Update();
      }  
   } 
   
   cnv_la->cd();
   leg->Draw();
   
   output = output_base + "-LA-bertini.gif";
   cnv_la->Print(output.c_str());

   return;
   
}

