#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/


std::vector<int> JoinCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)     
{          
    std::vector<int> responses;
    (void) params;
    
    try {  
        // Check if user is already on channel
        if (ch.isUserInChannel(cl)) {
            // Already on channel, return
            return responses;
        }
        
        // Check channel limits, keys, invite-only, etc.
        if (ch.isInviteOnly() && !ch.isUserInvited(cl)) {
            responses.push_back(473); // ERR_INVITEONLYCHAN
            return responses;
        }
        
        // Execute join
        ch.enterChannel(cl);       
        
        // Success responses
        responses.push_back(353); // RPL_NAMREPLY (names list)
        responses.push_back(366); // RPL_ENDOFNAMES
        
        // If channel has topic, send it
        if (!ch.getTopic().empty()) {
            responses.push_back(332); // RPL_TOPIC
        }
        
    } catch (std::exception &e) { 
        responses.push_back(403); // ERR_NOSUCHCHANNEL or generic error
    }
    
    return responses;
} 