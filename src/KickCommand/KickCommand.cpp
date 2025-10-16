#include "KickCommand.hpp"  
#include "Server.hpp"

KickCommand::KickCommand() 
{ 
     
}

void KickCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)
{    
    if(params.count("channel")  !=   0  || params.count("user") !=  0   )
    {  
        cl.addMsg(serverResponseFactory::getResp(461  ,  cl   , params ))  ;    
        return ;       
    }
    if(!ch->isOp(cl))  
    {  
        cl.addMsg(serverResponseFactory::getResp(482 , cl  , params) ) ;    
        return ;  
    }  
    if(!Server::getInstance().IsChannelExist(params["channel"]))
    {  
         cl.addMsg(serverResponseFactory::getResp(  403  ,  cl  , params  ))  ;   
         return  ;         
    }         
    Client target   = Server::getInstance().getUser(params["target"])    ;  
    if(!target.getChannel(params["channel"]))   
    { 
        cl.addMsg(serverResponseFactory::getResp(441  , cl  , params)) ;    
        return ;    
    }    
    std::string resp = ":" + cl.getNickName() + "!" + cl.getUsername() + "@" + SERVER_NAME +
                   " KICK #" + params["channel"] + " " + target.getNickName() + " :" + "You have been kicked";
    ch->broadcastMessage(cl  , resp  ) ;   
    ch->kickUser(cl  ,Server::getInstance().getUser(params["target"])  ,  "empty" )  ;   
}  
