#ifndef ModelConfigMapper_H
#define ModelConfigMapper_H 1

#include <string>
#include <vector>
#include <map>

// NOTE: NO namespace, to make it "fwk-neutral"
//
// NOTE (JVY, 6/11/2026): This is taken from G4VMP but some parts (EM, INCLXX, changing by ratio) 
//                        have been removed (for now)

class ModelConfigMapper
{

   public:
   
      // ctor & dtor
      //
      ModelConfigMapper();
      ~ModelConfigMapper();
      
      // member functions
      //
      void PrintDefaults( const std::string& ); 
      void PrintCurrentSettings( std::string model="all" );
      void RestoreDefaults( const std::string& );
      void RestoreAllDefaults();
      //
      void SetVerbosity( const std::string&, const bool& );
      //
      void ChangeParameter( const std::string&, const std::string&, const double&, bool verb=false );
      void ChangeParameter( const std::string&, const std::string&, const float&,  bool verb=false );
      void ChangeParameter( const std::string&, const std::string&, const int&,    bool verb=false ); 

      //double GetParamValue( const std::string&, const std::string& );
      //double GetParamDefault( const std::string&, const std::string& ) const;
            
   private:
   
      // private member functions
      //
      // HAD models
      //
      void FillBertiniDefaults();
      void FillPreCompoundDefaults();
      void FillFTFPDefaults();     
      //
      void FillConfigParamMapBertini();
      void FillConfigParamMapPreCo();
      void FillConfigParamMapFTFP();
      //
      void PrintBertiniSettings();
      void PrintPreCompoundSettings();
      void PrintFTFPSettings();
      //
      void ChangeParameterViaG4UI( const std::string&, const std::string&, const double&, bool verb=false );
      void ChangeParameterPreCo( const std::string&, const double&, bool verb=false );
      void ChangeParameterFTFP( const std::string&, const double&, bool verb=false );
      
      // data members
      //
      std::map< std::string, std::string >                        fNameConvention;
      std::string                                                 fBaseCommand; 
      std::map< std::string, std::map<std::string,std::string> >  fDEFAULTS; // NOTE: the reason of doing it as nested map(s) 
                                                                             // is because map has find(...) - simplifies search
									     // otherwise... would this below be more efficient ?
									     // std::map< std::string, std::vector<std::pair> >
      std::map< std::string, std::map<std::string, std::string> > fConfigParameters;

};


#endif
