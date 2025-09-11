#include "Reactor.hpp"  
#include "Server.hpp" 
#include "severResponsFactory.hpp"  
#include <string>  
#include "Client.hpp"  

int main() 
{   
    Client u(1 ,  "ascasc" , "sacaseic" , "ascasc"  ) ;    
    serverResponseFactory::respond(001 ,  u) ;      
    // Reactor::getInstance().Run() ;      
    
    // serverResponseFactory::respond(001 ,  data )   ;       
    // serverResponse::getInstanece()->respond(001 , test) ;   
}

