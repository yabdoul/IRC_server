#include "TopicCommand.hpp"   

std::vector<int> TopicCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)  
{   
    std::vector<int> responses;
    
    // Check if user is on channel
    if (!ch.isUserInChannel(cl)) {
        responses.push_back(442); // ERR_NOTONCHANNEL  
        return responses;
    }
    
    // If no topic parameter, return current topic
    if (params.find("topic") == params.end()) {
        if (ch.getTopic().empty()) {
            responses.push_back(331); // RPL_NOTOPIC
        } else {
            responses.push_back(332); // RPL_TOPIC  
        }
        return responses;
    }
    
    // Setting topic - check if topic protection is enabled
    if (ch.isTopicRestricted() && !ch.isOp(cl)) {
        responses.push_back(482); // ERR_CHANOPRIVSNEEDED
        return responses;
    }
    
    try {
        // Set topic
        ch.setTopic(cl, params["topic"]);
        // Topic change is broadcast as TOPIC message, not numeric
        
    } catch (std::exception& e) {
        responses.push_back(482); // ERR_CHANOPRIVSNEEDED (fallback)
    }
    
    return responses;
}
