#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/
void JoinCommand::execute()  
{  
    throw std::runtime_error("[JOIN]: IS a Channel Command") ;   
} ;      

void JoinCommand::exeChannel(Client  &cl ,  Channel &ch   , std::map<std::string ,  std::string >&params)     
{          
    (void)  params  ;   
    try{  
        ch.enterChannel(cl) ;  
        serverResponseFactory::respond(101 , cl) ;   
        /*
            [Implement th Server Response Here ! ,  serverResponse factory ]
        */
    } 
    catch(std::exception &e )  
    { 
        std::cerr<<e.what()<<std::endl;   
    }
} ; 