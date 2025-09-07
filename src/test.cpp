#include "Reactor.hpp"  
#include "Server.hpp" LEVEL SANTANA
#include "serverResponse.hpp"  
#include <string>  

int main() 
{ 
    
    // Reactor::getInstance().Run() ;    
    std::string  test("ascasc") ;  
    serverResponse::getInstance()->respond(001 , test) ;   
}

