
// #include "include/ModelConfig.hh"
#include "ModelConfig.hh"

void ModelConfig::Insert( const std::string& model, 
                          const std::string& param, 
			  const double&      value )
{

   // FOR NOW !!!
   // Check if "Default Physics" (no changes to G4's defaults whatsoever)
   // and store config(s) only if NOT
   //
   if ( fDefaultPhysics ) return; 
   
   if ( fModelConfig.find(model) == fModelConfig.end() )
   {
      fModelConfig.insert( std::pair< std::string, 
                                      std::vector< std::pair<std::string,double> > >( model,										                                                              std::vector< std::pair<std::string,double> >() ) );
   }
   std::map< std::string, std::vector< std::pair<std::string,double> > >::iterator itr=fModelConfig.find(model);
   
   (itr->second).push_back( std::pair<std::string,double>(param,value) );

   return;

} 

std::vector<std::string> ModelConfig::GetConfiguredModels() const
{

   std::vector<std::string> models;

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.begin();
   for ( ; itr!=fModelConfig.end(); ++itr )
   {
      models.push_back( itr->first );
   }
   
   return models;

}

const std::vector< std::pair<std::string,double> >* ModelConfig::GetConfig( const std::string& model ) const
{

   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelConfig.find(model);
   
   if ( itr == fModelConfig.end() )
   {
      // FIXME !! 
      // Needs to give a WARNING !!!
      //
      return NULL;
   }   

   return &(itr->second);
   
}

/*
const std::vector< std::pair<std::string,double> >* ModelConfig::GetDefault( const std::string& model ) const
{
   
   std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelDefault.find(model);

   if ( itr == fModelDefault.end() )
   {
      // FIXME !! 
      // Needs to give a WARNING !!!
      //
      return NULL;
   }   

   return &(itr->second);
   
}
*/

void ModelConfig::Print() const
{

   std::cout << " Class : ModelConfig " << std::endl;
   if ( fDefaultPhysics) 
   {
      std::cout << " Is Default Physics : " << fDefaultPhysics << std::endl;
      return;
   }
   
   
/*
   if ( fDefaultPhysics )
   {
      std::map< std::string, std::vector< std::pair<std::string,double> > >::const_iterator itr=fModelDefault.begin();
      for ( ; itr!=fModelDefault.end(); ++itr )
      {
      // models.push_back( itr->first );
         const std::vector< std::pair<std::string,double> >* moddef = GetDefault( itr->first );
         for ( size_t ip=0; ip<(*moddef).size(); ++ip )
	 {
            std::cout << " " << (*moddef)[ip].first << " : " << (*moddef)[ip].second << std::endl;
         }      
      }
   }
   else
   {
*/

      std::vector<std::string> modlist = GetConfiguredModels();

      std::cout << " Number of (re)configured models : " << modlist.size() << std::endl;
   
      for ( size_t im=0; im<modlist.size(); ++im )
      {
         std::cout << " Model : " << modlist[im] << std::endl;
         std::cout << " Parameters : " << std::endl;
         const std::vector< std::pair<std::string,double> >* modpars = GetConfig( modlist[im] );
         for ( size_t ip=0; ip<(*modpars).size(); ++ip )
         {
            std::cout << " " << (*modpars)[ip].first << " : " << (*modpars)[ip].second << std::endl;
         }      
      }
/*
   }
*/
   return;

}
