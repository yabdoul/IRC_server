#include "Reactor.hpp"  
#include "Server.hpp" 
#include "severResponsFactory.hpp"  
#include <string>  
#include "Client.hpp"  
#include "JoinCommand.hpp"  

int main() 
{   
    Client u(1 ,  "ascasc" , "sacaseic" , "ascasc"  ) ;        
    
    Channel ch  ;      
    Client c(2  ,  "ascasc" ,  "ascasc "  , "ascasc") ;      
    ch.ExecuteCommand ( new  JoinCommand() ,  u ,   std::map<std::string , std::string> ()) ; 
     serverResponseFactory::respond(001 ,  u) ;      
    // Reactor::getInstance().Run() ;      
    
    // serverResponseFactory::respond(001 ,  data )   ;       
    // serverResponse::getInstanece()->respond(001 , test) ;   
}

