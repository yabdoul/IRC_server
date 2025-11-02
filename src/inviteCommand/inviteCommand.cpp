#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::exeChannel(Client &cl , Channel *ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    if (params.find("nickname") == params.end() || params.at("nickname").empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }

    if (!ch) {
        cl.addMsg(serverResponseFactory::getResp(403, cl, params));
        return;
    }

    try {
        Client &targetClient = Server::getInstance().getUser(params.at("nickname"));

        
        if (!cl.getChannel(ch->getName())) {
            cl.addMsg(serverResponseFactory::getResp(442, cl, params, ch)); 
            return;
        }

        
        if (ch->isInviteOnly() && !ch->isOp(cl)) {
            cl.addMsg(serverResponseFactory::getResp(482, cl, params, ch)); 
            return;
        }

        
        ch->inviteUser(cl, targetClient);
        cl.addMsg(serverResponseFactory::getResp(341, cl, params, ch)); 
    } catch (std::exception &e) {
        
        cl.addMsg(serverResponseFactory::getResp(401, cl, params));
        return;
    }
} ;     