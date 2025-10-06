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
            
            targetChannel->broadcastMessage(privmsgMsg, &sender);
            
        } catch (std::exception& e) {
            sender.addMsg(serverResponseFactory::getResp(403, sender , params  )); 
        }
        
    } else if (targetType == "user") {
        // User-to-user message
        if (params.find("nickname") == params.end()) {
            sender.addMsg(serverResponseFactory::getResp(461, sender ,  params   ));  
            return;
        }
        
        try {
            std::string targetNick = params["nickname"];
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
 

void PrivmsgCommand:: exeChannel(Client &sender , Channel *ch  , std::map<std::string ,  std::string>&params )
{     
    ch->getClients() ;   
    if(!ch)  
    {  
        std::cerr<<"channel Not found "  ; 
        return ;   
    }
    if(!sender.getChannel(ch->getName()))     
    {  
        sender.addMsg(serverResponseFactory::getResp(403 ,  sender , params , ch )) ;     
        return ;     
    }  
    else{ 
        ch->broadcastMessage(params.at("message") )  ;           
    }
}