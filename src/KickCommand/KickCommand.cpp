#include "KickCommand.hpp"  
#include "Server.hpp"

KickCommand::KickCommand() 
{ 
     
}

void KickCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)
{      
    if (params.count("channel") == 0 || params.count("target") == 0) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }

    if (!ch) {  
        std::cout<<"channel null"<<std::endl ;   
        cl.addMsg(serverResponseFactory::getResp(403, cl, params));
        return;
    }

    if (!ch->isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482, cl, params));
        return;
    }

    if (!Server::getInstance().IsChannelExist(params.at("channel"))) {  
        std::cout<<"channel doenst  exist"<<std::endl  ;    
        cl.addMsg(serverResponseFactory::getResp(403, cl, params));
        return;
    }

    try {
        Client &target = Server::getInstance().getUser(params.at("target"));
        if (!target.getChannel(params.at("channel"))) {
            cl.addMsg(serverResponseFactory::getResp(441, cl, params)); // ERR_USERNOTINCHANNEL
            return;
        }

        std::string resp = ":" + cl.getNickName() + "!" + cl.getUsername() + "@" + SERVER_NAME +
                           " KICK #" + params.at("channel") + " " + target.getNickName() + " :You have been kicked";
        ch->broadcastMessage(cl, resp);
        ch->kickUser(cl, target, "kicked");
        std::cout << "user kicked" << std::endl;
    } catch (std::exception &e) {
        cl.addMsg(serverResponseFactory::getResp(401, cl, params));
        return;
    }
}  
