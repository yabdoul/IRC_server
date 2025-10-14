#include "userCommand.hpp"  
#include "severResponsFactory.hpp"  


void userCommand::execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )    
{     
    sender.setUser(params["username"]) ;   
    sender.addMsg(serverResponseFactory::getResp(001  ,   sender  ,  params ))  ;    
    sender.addMsg(serverResponseFactory::getResp(002 ,  sender ,  params)) ;   
    sender.addMsg(serverResponseFactory::getResp(003  , sender  , params ) )  ;   
 } ;     