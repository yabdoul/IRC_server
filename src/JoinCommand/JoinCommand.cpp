#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/
  

std::vector<int> JoinCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)     
{          
    std::vector<int> responses;
    (void) params;
    
    try {  
        if (ch.isUserInChannel(cl)) {
            return responses;
        }
        
        if (ch.isInviteOnly() && !ch.isUserInvited(cl)) {
            responses.push_back(473); 
            return responses;
        }
        ch.enterChannel(cl);       
        responses.push_back(353); 
        responses.push_back(366);
        if (!ch.getTopic().empty()) {
            responses.push_back(332);
        }
    } catch (std::exception &e) { 
        responses.push_back(403); 
    }
    return responses;
} 