#include "../test23/shared/rootanalysis/include/ModelConfig.hh"
// #include "TSystem.h"
#include "TFile.h"

// NOTE: *INTERACTIVELY* it works just fine if loading via gSystem
//       but via script it needs to be done as follows:
//
R__LOAD_LIBRARY(libModelConfig.so)

void ReadModelConfig()
{

   // gSystem->Load("libModelConfig.so");
   
   TFile* f = TFile::Open("harp-histo/protonC8.0GeVbertiniUniv30.root");
   f->ls();
   
   ModelConfig* mc = nullptr;
   
   f->GetObject("modelconfig",mc);
   
   mc->Print();

   return;
   
}
