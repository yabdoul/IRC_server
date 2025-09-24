#include "inviteCommand.hpp"   
#include  "Server.hpp"  

std::vector<int> inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    if (params.find("nickname") == params.end()) {
        responses.push_back(461); 
        return responses;
    }
    
    try {
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);
        
        if (ch.isUserInChannel(targetClient)) {
            responses.push_back(443);
            return responses;
        }
        

        if (!ch.isUserInChannel(cl)) {
            responses.push_back(442);
            return responses;
        }
        
        if (ch.isInviteOnly() && !ch.isOp(cl)) {
            responses.push_back(482);
            return responses;
        }
        
        ch.inviteUser(cl, targetClient);
        responses.push_back(341); 
        
    } catch (std::exception& e) {
        responses.push_back(401); 
    }
    
    return responses;
} ;     