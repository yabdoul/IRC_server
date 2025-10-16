#include "userPrivMsg.hpp"   
#include "severResponsFactory.hpp" 
#include "Server.hpp"  

void userPrivMsg::execute(Client& sender, std::map<std::string, std::string>& params)
{     
    std::cout<<"entred the proper function -=---------"<<std::endl ;    
    
    if (params.find("message") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(412, sender ,  params  ));
        return;
    }
    
    if (params.find("target_type") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(461, sender  ,  params  ));  
        return;
    }
    
    std::string message = params["message"];
    std::string targetType = params["target_type"];
    
    if (targetType == "channel") {
        if (params.find("channel") == params.end()) {
            sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   )); 
            return;
        }
        
        try {
            std::string channelName = params["channel"];
            Channel * targetChannel = Server::getInstance().IsChannelExist(channelName);
            
            if (!sender.getChannel(targetChannel->getName())) {
                sender.addMsg(serverResponseFactory::getResp(404, sender ,  params  ));  
                return;
            }
            
            std::string privmsgMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                                    "@localhost PRIVMSG #" + channelName + " :" + message + "\r\n";
            
            targetChannel->broadcastMessage( sender ,  privmsgMsg);
            
        } catch (std::exception& e) {
            sender.addMsg(serverResponseFactory::getResp(403, sender , params  )); 
        }
        
    } else if (targetType == "user") {   
        if (params.find("nickname") == params.end()) {
            sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  
            return;
        }
        
        try {
            std::string targetNick = params["nickname"];   
            std::cout<<"target nick is "<<std::endl ;    
            Client& targetUser = Server::getInstance().getUser(targetNick);
            std::string privmsgMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                                    "@localhost PRIVMSG " + targetNick + " :" + message + "\r\n";
            targetUser.addMsg(privmsgMsg)    ;
        } catch (std::exception& e) {
            sender.addMsg(serverResponseFactory::getResp(401, sender  , params  ));  
        }
        
    } else {
        sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  
    }
}   