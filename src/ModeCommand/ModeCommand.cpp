#include "ModeCommand.hpp"   

std::vector<int> ModeCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)  
{   
    std::vector<int> responses;
    
    // if user is on channel
    if (!ch.isUserInChannel(cl)) {
        responses.push_back(442); // ERR_NOTONCHANNEL  
        return responses;
    }
    
    // If no mode parameter, return current modes
    if (params.find("mode") == params.end()) {
        responses.push_back(324); // RPL_CHANNELMODEIS
        return responses;
    }
    
    // Setting mode - check operator privileges
    if (!ch.isOp(cl)) {
        responses.push_back(482); // ERR_CHANOPRIVSNEEDED
        return responses;
    }
    
    try {
        std::string mode = params["mode"];
        std::string modeParam = "";
        
        if (params.find("mode_param") != params.end()) {
            modeParam = params["mode_param"];
        }
        
        // Execute mode change
        ch.setMode(cl, mode, modeParam);
        
    } catch (std::exception& e) {
        responses.push_back(501); // ERR_UNKNOWNMODE
    }
    
    return responses;
}
