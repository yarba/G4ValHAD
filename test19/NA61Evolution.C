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
#include "TGraphErrors.h"

std::string TEST_NAME="test19";

#include "../test23/shared-root-macros/SplitString.C"
#include "../test23/shared-root-macros/ReadNA61Data.C"
#include "../test23/shared-root-macros/Chi2Calc.C"
#include "../test23/shared-root-macros/Chi2CalcNA61.C"

#include "../test23/shared-root-macros/REGRESSION_TEST.h"

void NA61Evolution( std::string model, std::string sec )
{

   std::string cname = "cnv_" + model + "_proton31GeV_C_" + sec;
   TCanvas* cnv = new TCanvas( cname.c_str(), "", 920, 690 );
   
   std::string pname = "pad_" + model + "_proton31GeV_C_" + sec;
   TPad* pad = new TPad( pname.c_str(), "", 0.01, 0.01, 0.99, 0.97 );
   pad->Divide( 4, 3, 0.01, 0.01 );
   
   TLegend* leg = new TLegend( 0.01, 0.01, 0.99, 0.99 );
   leg->SetFillColor(kWhite);
   leg->SetBorderSize(0);
   TLegendEntry* lentry = 0; 
   
   lentry = leg->AddEntry("","NA61 data","p");
   lentry->SetMarkerStyle(22);
   lentry->SetMarkerColor(kBlue);
   lentry->SetMarkerSize(1.5);
   lentry->SetTextFont(62);
   lentry->SetTextColor(kBlue);

   cnv->cd();
   pad->Draw();
   
   // open up histo files
   //
   std::string location = "";
   
   TFile* files[NVersions];
   TH1F*  h[NVersions];
   
   for ( int iv=0; iv<NVersions; iv++ )
   {
      if ( Versions[iv] == CurrentVersion )
      {
         location = ".";
      }
      else
      {
         location = regre_test_dir + "/test19/" + Versions[iv];
      }
      std::string histofile = location + "/na61-histo/protonC31.0GeV" + model + ".root";       
      files[iv] = new TFile( histofile.c_str() );
      
      double chi2 = 0.;
      int NDF = 0;
      std::ostringstream os;
      chi2 = Chi2NA61IntegralData2015( "proton", "C", sec, model, NDF, Versions[iv] );

      os << (chi2/NDF);
      std::string txt1 = "#chi^{2}/NDF = " + os.str();
      std::string vv = Versions[iv].erase( 0, std::strlen("geant4-") );
      txt1 += ( " " + vv );
/*
      TLatex* ltxt1 = new TLatex( 0.01, 0.85-(2.*0.055*iv), txt1.c_str() );
      ltxt1->SetTextSize(0.055);
      ltxt1->SetTextColor( ColorVersion[iv] );
      pad->cd( pad->GetListOfPrimitives()->GetSize() );
      ltxt1->Draw();
*/
      lentry = leg->AddEntry( "", txt1.c_str(), "L" );
      lentry->SetLineColor( ColorVersion[iv] );
      lentry->SetLineWidth(3);
      lentry->SetTextFont(62);
      lentry->SetTextColor( ColorVersion[iv] );
   } 
   
   pad->cd( pad->GetListOfPrimitives()->GetSize() );
   leg->Draw();  
      
//   readMomSpectrum( "proton", "C", secondary, "0", "10", "../test23/na61-exp-data/Pub-2015/" );

   // go over the 1st file in the list
   //
   TList* keys = files[0]->GetListOfKeys();
   int nkeys = keys->GetEntries();
   int icnt = 0;
   for ( int ik=0; ik<nkeys; ++ik )
   {
      if ( icnt >= (pad->GetListOfPrimitives()->GetSize()-1) ) break;
      TKey* key = (TKey*)(keys->At(ik));
      std::string key_class_name = key->GetClassName();
      if ( key_class_name.find("TH1F") == std::string::npos ) continue;
      std::string key_name = key->GetName();
      if ( key_name.find(sec) == std::string::npos ) continue;
      if ( key_name.find("Mult") != std::string::npos ) continue;
      if ( key_name.find("kplus2piplus") != std::string::npos ) continue;
      h[0] = (TH1F*)( files[0]->Get(key_name.c_str()) );
      h[0]->SetStats(0);
      h[0]->SetLineColor(ColorVersion[0]);
      h[0]->SetLineWidth(2);
      pad->cd(icnt+1);
      gPad->Update();
      pad->Update();
      cnv->Update(); 
      float ymax = -1.;
      int imax = h[0]->GetMaximumBin();
      if ( ymax < (h[0]->GetBinContent(imax)+2.*h[0]->GetBinError(imax)) )
         ymax = h[0]->GetBinContent(imax)+2.*h[0]->GetBinError(imax);
      h[0]->GetYaxis()->SetRangeUser( 0., ymax );
      h[0]->Draw("histE1");
      gPad->Update();
      pad->Update();
      cnv->Update();
      for ( int iv=1; iv<NVersions; ++iv )
      {
         h[iv] =(TH1F*)( files[iv]->Get( key_name.c_str()) );
	 h[iv]->SetStats(0);
	 h[iv]->SetLineColor(ColorVersion[iv]);
	 h[iv]->SetLineWidth(2);
	 imax = h[iv]->GetMaximumBin();
         if ( ymax < (h[iv]->GetBinContent(imax)+2.*h[iv]->GetBinError(imax)) )
            ymax = h[iv]->GetBinContent(imax)+2.*h[iv]->GetBinError(imax);
         h[iv]->GetYaxis()->SetRangeUser( 0., ymax ); 
	 for ( int iv1=0; iv1<iv; ++iv1 )
	 {
	    h[iv1]->GetYaxis()->SetRangeUser( 0., ymax );
	 }
         gPad->Update();
         pad->Update();
         cnv->Update();
	 h[iv]->DrawCopy("histE1same" );    
         gPad->Update();
         pad->Update();
         cnv->Update();
      }      	
      std::vector<std::string> tokens;
      SplitString( key_name, "_", tokens );
      int ntk = tokens.size();
//      std::cout << tokens[ntk-2] << "..." << tokens[ntk-1] << std::endl;
      readMomSpectrum( "proton", "C", sec, 
                       tokens[ntk-2], tokens[ntk-1], 
                       "../test23/na61-exp-data/Pub-2015/" );
      TGraphErrors* gdata = getNA61AsGraph();
      float gmax = gdata->GetMaximum();
//      std::cout << "..." << gmax << "..." << std::endl;
      if ( ymax < gdata->GetMaximum() ) ymax = gdata->GetMaximum();
      gdata->SetMarkerColor(kBlue);
      gdata->SetMarkerStyle(22);
      gdata->SetMarkerSize(1.5);
      gdata->GetYaxis()->SetRangeUser( 0., ymax );
      for ( int iv=0; iv<NVersions; ++iv )
      {
         h[iv]->GetYaxis()->SetRangeUser( 0., ymax );
      }
      gPad->Update();
      pad->Update();
      cnv->Update();
      gdata->Draw("psame");
      h[0]->SetLineWidth(1);
      h[0]->Draw("histE1same");
      gPad->Update();
      pad->Update();
      cnv->Update();
      icnt++;
   }
   
   std::string output = cnv->GetName();
   output += ".png";
   cnv->Print( output.c_str() );
   
   return;

}
