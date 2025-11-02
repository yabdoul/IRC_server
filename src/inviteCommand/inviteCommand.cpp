#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::exeChannel(Client &cl , Channel *ch  , std::map<std::string ,  std::string >&params   )  
{   
    std::vector<int> responses;
    if (params.find("nickname") == params.end() || params.at("nickname").empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // ERR_NEEDMOREPARAMS
        return;
    }

    if (!ch) {
        cl.addMsg(serverResponseFactory::getResp(403, cl, params));
        return;
    }

    try {
        Client &targetClient = Server::getInstance().getUser(params.at("nickname"));

        // If the inviter is not on the channel
        if (!cl.getChannel(ch->getName())) {
            cl.addMsg(serverResponseFactory::getResp(442, cl, params, ch)); // ERR_NOTONCHANNEL
            return;
        }

        // If channel is invite-only and inviter is not op
        if (ch->isInviteOnly() && !ch->isOp(cl)) {
            cl.addMsg(serverResponseFactory::getResp(482, cl, params, ch)); // ERR_CHANOPRIVSNEEDED
            return;
        }

        // Invite the user
        ch->inviteUser(cl, targetClient);
        cl.addMsg(serverResponseFactory::getResp(341, cl, params, ch)); // RPL_INVITING
    } catch (std::exception &e) {
        // No such nick
        cl.addMsg(serverResponseFactory::getResp(401, cl, params));
        return;
    }
} ;     