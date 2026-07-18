#ifndef ModelConfig_H
#define ModelConfig_H

#include<string>
#include <vector>
#include <map>

#include <iostream>

class ModelConfig {

   // NOTE (JVY): In principle, one can have an std::string model name and 
   //             an std::vector<std::pair<...>> set of parameters for it,
   //             and that'd be just fine for a single model.
   //             However, for a physics list having some infrastructure
   //             for multiple models might be useful... if we want it that way !
   //             Even if it's not a physics list but a combo of models,
   //             like FTFP or Bertini+PreCo, it might be useful.
   //             it's something to think about...


   public:
      ModelConfig() : fDefaultPhysics(true), fName("modelconfig") {}
      ModelConfig( const bool& flag ) : fDefaultPhysics(flag), fName("modelconfig") {}
      ModelConfig( const ModelConfig& rhs ) : fDefaultPhysics(rhs.fDefaultPhysics),
                                              fName(rhs.fName),
                                              fModelConfig(rhs.fModelConfig) {} 
      ~ModelConfig() {}
      
      void Insert( const std::string&, const std::string&, const double& );
      // void InsertDefault( const std::string&, const std::string&, const double& );
      
      void                                                SetName( std::string& name ) { fName=name; return; }
      bool                                                IsDefaultPhysics() const { return fDefaultPhysics; }
      std::string                                         GetName() const { return fName; }
      std::vector<std::string>                            GetConfiguredModels() const;      
      const std::vector< std::pair<std::string,double> >* GetConfig( const std::string& ) const;
      // const std::vector< std::pair<std::string,double> >* GetDefault( const std::string& ) const;
      void                                                Print() const;

   private:
      bool        fDefaultPhysics;
      std::string fName;
      //
      // NOTE-0 (JVY): THIS IS PORT FROM G4VMP thus some comments are preserved fo now,
      //               WITH REFERENCES to ART, etc.; hopefully, will be removed later
      //
      // NOTE-1(JVY): the reason of using this construct is because there's already
      //            a "standard" (Art's) dictionary for it (art/Persistency/StdDictionaries)
      //
      // NOTE-2(JVY): at present, only those parameters different from DEFAULTS will be stored
      //              although in the future we might consider a bit more extensive options
      //
      std::map< std::string, std::vector< std::pair<std::string,double> > > fModelConfig;
      // std::map< std::string, std::vector< std::pair<std::string,double> > > fModelDefault;

};


#endif
