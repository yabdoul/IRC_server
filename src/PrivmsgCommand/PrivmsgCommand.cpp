#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include "Reactor.hpp"  

PrivmsgCommand::PrivmsgCommand()
{
}

PrivmsgCommand::~PrivmsgCommand()
{
}


 

void PrivmsgCommand:: exeChannel(Client &sender , Channel *ch  , std::map<std::string ,  std::string>&params )
{       
    
    if (!ch) {
        sender.addMsg(serverResponseFactory::getResp(403, sender, params)) ;
        return;
    }
    if (!sender.getChannel(ch->getName())) {
        sender.addMsg(serverResponseFactory::getResp(442, sender, params, ch)); 
        return;
    }

    
    if (params.find("message") == params.end() || params["message"].empty()) {
        sender.addMsg(serverResponseFactory::getResp(412, sender, params)); 
        return;
    }

    
    std::string msg = ":" + sender.getNickName() + "!" + sender.getUsername() + "@" + SERVER_NAME
                      + " PRIVMSG #" + ch->getName() + " :" + params["message"];
    ch->broadcastMessage(sender, msg, &sender);
}