#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::execute() { 
      throw std::runtime_error("[INVITE] : is a Channel Command") ;   
}   
  

void  inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    
    // Validate parameters
    if (params.find("nickname") == params.end()) {
        responses.push_back(461); // ERR_NEEDMOREPARAMS
        return responses;
    }
    
    try {
        // Check if user exists
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);
        
        // iff user is already on channel
        if (ch.isUserInChannel(targetClient)) {
            responses.push_back(443); // ERR_USERONCHANNEL
            return responses;
        }
        
        // if inviter is on channel
        if (!ch.isUserInChannel(cl)) {
            responses.push_back(442); // ERR_NOTONCHANNEL  
            return responses;
        }
        
        // if channel is invite-only
        if (ch.isInviteOnly() && !ch.isOp(cl)) {
            responses.push_back(482); // ERR_CHANOPRIVSNEEDED
            return responses;
        }
        
        // Execute invite
        ch.inviteUser(cl, targetClient);
        responses.push_back(341); // RPL_INVITING
        
    } catch (std::exception& e) {
        responses.push_back(401); // ERR_NOSUCHNICK
    }
    
    return responses;
} ;     