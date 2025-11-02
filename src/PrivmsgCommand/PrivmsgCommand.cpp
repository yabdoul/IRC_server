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
    std::cout<<"Called by"<<sender.getNickName()<<std::endl ;   
    // Validate channel pointer first
    if (!ch) {
        std::cerr << "Privmsg: channel pointer is null" << std::endl;
        sender.addMsg(serverResponseFactory::getResp(403, sender, params));
        return;
    }

    // Ensure sender is on the channel
    if (!sender.getChannel(ch->getName())) {
        sender.addMsg(serverResponseFactory::getResp(442, sender, params, ch)); // ERR_NOTONCHANNEL
        return;
    }

    // Validate message content
    if (params.find("message") == params.end() || params["message"].empty()) {
        sender.addMsg(serverResponseFactory::getResp(412, sender, params)); // ERR_NOTEXTTOSEND
        return;
    }

    // Build and broadcast message to channel
    std::string msg = ":" + sender.getNickName() + "!" + sender.getUsername() + "@" + SERVER_NAME
                      + " PRIVMSG #" + ch->getName() + " :" + params["message"];
    ch->broadcastMessage(sender, msg, &sender);
}