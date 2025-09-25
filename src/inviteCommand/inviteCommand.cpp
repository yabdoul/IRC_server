#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    
    if (params.find("nickname") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(461 ,  cl )) ;   
    }
    
    try {
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);

        if (ch.isUserInChannel(targetClient)) {  
            cl.addMsg(serverResponseFactory::getResp(443 ,  cl ))  ;  
        }
        
        if (!ch.isUserInChannel(cl)) {
            cl.addMsg(serverResponseFactory::getResp(442 ,  cl ))  ;  
        }
        
        if (ch.isInviteOnly() && !ch.isOp(cl)) {
            cl.addMsg(serverResponseFactory::getResp(482 ,  cl ))  ;  
        }
        ch.inviteUser(cl, targetClient);
        cl.addMsg(serverResponseFactory::getResp(341 ,  cl ))  ;  
    } catch (std::exception& e) {
        responses.push_back(401); 
    }
} ;     