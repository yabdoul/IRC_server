#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::exeChannel(Client &cl , Channel *ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    
    if (params.find("nickname") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(461 ,  cl  , params   )) ;   
    }
    
    try {
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);

        if (cl.getChannel(ch->getName())) {  
            cl.addMsg(serverResponseFactory::getResp(443 ,  cl  ,  params   ))  ;  
        }
        
        if (!cl.getChannel(ch->getName())) {
            cl.addMsg(serverResponseFactory::getResp(442 ,  cl   , params  ))  ;  
        }
        
        if (ch->isInviteOnly() && !ch->isOp(cl)) {
            cl.addMsg(serverResponseFactory::getResp(482 ,  cl  ,  params  ))  ;  
        }
        ch->inviteUser(cl, targetClient);
        cl.addMsg(serverResponseFactory::getResp(341 ,  cl ,  params  ))  ;  
    } catch (std::exception& e) {
        responses.push_back(401); 
    }
} ;     