#include "PartCommand.hpp"
#include "Server.hpp"

void PartCommand::exeChannel(Client& cl, Channel& ch, std::map<std::string, std::string>& params)
{
    try {
        if (!ch.isUserInChannel(cl)) {
            // 442 ERR_NOTONCHANNEL
            cl.addMsg(serverResponseFactory::getResp(442, cl));
            return;
        }
        
        // Get optional part message
        std::string partMessage = "Leaving";
        if (params.find("message") != params.end()) {
            partMessage = params["message"];
        }
        
        // Create PART message to broadcast: :nick!user@host PART #channel :message
        std::string partMsg = ":" + cl.getNickname() + "!" + cl.getUsername() + 
                              "@localhost PART #" + ch.getName() + " :" + partMessage + "\r\n";
        
        // Broadcast PART message to all channel members (including the leaving user)
        ch.broadcastMessage(partMsg);
        
        // Remove user from channel
        ch.removeUser(cl);
        
    } catch (std::exception& e) {
        // 403 ERR_NOSUCHCHANNEL (fallback error)
        cl.addMsg(serverResponseFactory::getResp(403, cl));
    }
}
