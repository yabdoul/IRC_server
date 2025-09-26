#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include "Reactor.hpp"  

PrivmsgCommand::PrivmsgCommand()
{
}

PrivmsgCommand::~PrivmsgCommand()
{
}

void PrivmsgCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{
    if (params.find("message") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(412, sender ,  params  ));
        return;
    }
    
    if (params.find("target_type") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(461, sender  ,  params  ));  // ERR_NEEDMOREPARAMS
        return;
    }
    
    std::string message = params["message"];
    std::string targetType = params["target_type"];
    
    if (targetType == "channel") {
        if (params.find("channel") == params.end()) {
            sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  // ERR_NEEDMOREPARAMS
            return;
        }
        
        try {
            std::string channelName = params["channel"];
            Channel targetChannel = Server::getInstance().IsChannelExist(channelName);
            
            if (!targetChannel.isUserInChannel(sender)) {
                sender.addMsg(serverResponseFactory::getResp(404, sender ,  params  ));  
                return;
            }
            
            // Create PRIVMSG message: :nick!user@host PRIVMSG #channel :message
            std::string privmsgMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                                    "@localhost PRIVMSG #" + channelName + " :" + message + "\r\n";
            
            // Broadcast to channel members (exclude sender)
            targetChannel.broadcastMessage(privmsgMsg, &sender);
            
        } catch (std::exception& e) {
            sender.addMsg(serverResponseFactory::getResp(403, sender , params  ));  // ERR_NOSUCHCHANNEL
        }
        
    } else if (targetType == "user") {
        // User-to-user message
        if (params.find("nickname") == params.end()) {
            sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  // ERR_NEEDMOREPARAMS
            return;
        }
        
        try {
            std::string targetNick = params["nickname"];
            Client& targetUser = Server::getInstance().getUser(targetNick);
            std::string privmsgMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                                    "@localhost PRIVMSG " + targetNick + " :" + message + "\r\n";
            targetUser.addMsg(privmsgMsg)    ;
        } catch (std::exception& e) {
            sender.addMsg(serverResponseFactory::getResp(401, sender  , params  ));  // ERR_NOSUCHNICK
        }
        
    } else {
        sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  // ERR_NEEDMOREPARAMS
    }
}
