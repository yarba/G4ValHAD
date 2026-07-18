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
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TRefArray.h"
#include "TStyle.h"
#include "TGraph.h"

const int NModels = 2;
std::string Models[2] = { "bertini", /* "ftfp", "ftfp_tune3", */ "bertini.11.2" };

void ConvertRootFiles( std::string beam, std::string target, std::string energy )
{

   TH1::SetDefaultSumw2();

   std::string oloc = "original-root-files/"; 
   std::string cloc = "converted-root-files/";
   
   std::vector<std::string> secs;
   secs.push_back("proton");
   secs.push_back("neutron");
   
   std::vector<std::string> angles;
   angles.push_back("10.1");
   angles.push_back("15.0");
   angles.push_back("19.8");
   angles.push_back("24.8");
   angles.push_back("29.5");
   angles.push_back("34.6");
   angles.push_back("39.6");
   angles.push_back("44.3");
   angles.push_back("49.3");
   angles.push_back("54.2");
   angles.push_back("59.1");
   angles.push_back("64.1");
   angles.push_back("69.1");
   angles.push_back("74.1");
   angles.push_back("79.1");
   angles.push_back("84.1");
   angles.push_back("89.0");
   angles.push_back("98.9");
   angles.push_back("108.9");
   angles.push_back("119.0");
   angles.push_back("129.1");
   angles.push_back("139.1");
   angles.push_back("149.3");
   angles.push_back("159.6");
   angles.push_back("161.4");
   angles.push_back("165.5");
   angles.push_back("169.5");
   angles.push_back("173.5");
   angles.push_back("177.0");
   
      
   for ( int im=0; im<NModels; ++im )
   {
      std::string ohfile = oloc + beam + target + Models[im] + energy + "GeV.root"; 
      std::string chfile = cloc + beam + target + energy + "GeV" + Models[im] + ".root";
   
      TFile* ofile = TFile::Open(ohfile.c_str());
      TFile* cfile = TFile::Open(chfile.c_str(),"RECREATE");
   
      for (size_t is=0; is<secs.size(); ++is )
      {
         for (size_t ia=0; ia<angles.size(); ++ia)
         {
            std::string ohname = "KE" + secs[is] + "0" + beam + target + Models[im] + energy + "GeV";
            int counter = 5 - angles[ia].length();
            for ( int il=0; il<counter; il++ )
            {
               ohname += " "; // pad up for the fact that initially sec.angle was supposed to be char[6] - no more, no less...
            }
            ohname += angles[ia];
	    TH1F* oh = (TH1F*)ofile->Get(ohname.c_str());
            std::string chname = secs[is] + "_at_" + angles[ia] + "deg";
	    TH1F* ch = (TH1F*)oh->Clone(chname.c_str());
	    std::string chtitle = beam + " + " + target + " -> " + secs[is] + " + X, theta=" + angles[ia];
	    ch->SetTitle(chtitle.c_str());
	    cfile->cd();
	    ch->Write();
         }
      }
   }
      
   return;

}
