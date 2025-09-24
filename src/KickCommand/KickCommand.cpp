#include "KickCommand.hpp"  
#include "Server.hpp"

KickCommand::KickCommand() 
{ 
     
}

std::vector<int> KickCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)
{
    std::vector<int> responses;
    
    // Validate params
    if (params.find("nickname") == params.end()) {
        responses.push_back(461); // ERR_NEEDMOREPARAMS
        return responses;
    }
    
    try {
        // if kicker is on channel
        if (!ch.isUserInChannel(cl)) {
            responses.push_back(442); // ERR_NOTONCHANNEL  
            return responses;
        }
        
        // kicker is operator
        if (!ch.isOp(cl)) {
            responses.push_back(482); // ERR_CHANOPRIVSNEEDED
            return responses;
        }
        
        // target user exists
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);
        
        // targt is on the channel
        if (!ch.isUserInChannel(targetClient)) {
            responses.push_back(441); // ERR_USERNOTINCHANNEL
            return responses;
        }
        
        // kick reason
        std::string reason = "No reason given";
        if (params.find("reason") != params.end()) {
            reason = params["reason"];
        }
        
        // Execute kick
        ch.kickUser(cl, targetClient, reason);
        
    } catch (std::exception& e) {
        responses.push_back(401); // ERR_NOSUCHNICK
    }
    
    return responses;
}  
