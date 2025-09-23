#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/

std::vector<int>   JoinCommand::exeChannel(Client  &cl ,  Channel &ch   , std::map<std::string ,  std::string >&params)     
{          
    (void)  params  ;   
    try{  
        ch.enterChannel(cl) ;       
        
        /*
            [Implement th Server Response Here ! ,  serverResponse factory ]
        */
    } 
    catch(std::exception &e )  
    { 
        std::cerr<<e.what()<<std::endl;   
    }  
    return std::vector<int>() ;   
} ; 