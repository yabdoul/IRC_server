#include "KickCommand.hpp"  
#include "Server.hpp"

KickCommand::KickCommand() 
{ 
     
}

void KickCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)
{
    // Check if nicknames parameter is provided
    if (params.find("nicknames") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(461 ,  cl  ,  params  ));  // ERR_NEEDMOREPARAMS
        return;
    }
    
    // Check if user is in the channel
    if (cl.getChannel(ch.getName())) {
        cl.addMsg(serverResponseFactory::getResp(442 ,   cl  ,  params  ));  // ERR_NOTONCHANNEL
        return; 
    }
    
    // Check if user is channel operator
    if (!ch.isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482 ,  cl , params   ));  // ERR_CHANOPRIVSNEEDED
        return;
    }
    
    // Get kick reason
    std::string reason = "No reason given";
    if (params.find("reason") != params.end()) {
        reason = params["reason"];
    }
    
    // Parse comma-separated nicknames
    std::string nicknames = params["nicknames"];
    std::vector<std::string> targetNicks;
    std::string currentNick = "";
    
    for (size_t i = 0; i < nicknames.length(); ++i) {
        if (nicknames[i] == ',') {
            if (!currentNick.empty()) {
                targetNicks.push_back(currentNick);
                currentNick = "";
            }
        } else {
            currentNick += nicknames[i];
        }
    }
    if (!currentNick.empty()) {
        targetNicks.push_back(currentNick);
    }
    
    for (size_t i = 0; i < targetNicks.size(); ++i) {
        try {
            Client& targetClient = Server::getInstance().getUser(targetNicks[i]);
            
            if (cl.getChannel(ch.getName())) {
                cl.addMsg(serverResponseFactory::getResp(441 ,  cl   ,  params      ));  
                continue; 
            }
            
            ch.kickUser(cl, targetClient, reason);
            
        } catch (std::exception& e) {
            cl.addMsg(serverResponseFactory::getResp(401 ,  cl ,  params   ));  
        }
    }
    
}  
