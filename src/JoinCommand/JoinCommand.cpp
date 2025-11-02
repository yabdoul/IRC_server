#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/

void JoinCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)     
{            
    (void )params ;   
    try {    
        if (!ch) {
            cl.addMsg(serverResponseFactory::getResp(403, cl, params));
            return;
        }

        // Invite only check
        if (ch->isInviteOnly() && !ch->isUserInvited(cl)) {
            cl.addMsg(serverResponseFactory::getResp(473, cl, params, ch));
            return;
        }

        // Key protected channel
        if (ch->isLocked()) {
            if (params.count("key") == 0 || params.at("key").empty()) {
                cl.addMsg(serverResponseFactory::getResp(475, cl, params, ch));
                return;
            }

            if (params.at("key") != ch->getKey()) {
                cl.addMsg(serverResponseFactory::getResp(475, cl, params, ch));
                return;
            }
        }

        // Finally attempt to enter the channel
        ch->enterChannel(&cl);
        cl.addMsg(serverResponseFactory::getResp(1001, cl, params, ch));
        cl.addMsg(serverResponseFactory::getResp(353, cl, params, ch));
        cl.addMsg(serverResponseFactory::getResp(366, cl, params, ch));
        if (!ch->getTopic().empty()) {
            cl.addMsg(serverResponseFactory::getResp(332, cl, params, ch));
        }
    } catch (std::exception &e) {
        // Generic failure when channel operations throw
        cl.addMsg(serverResponseFactory::getResp(403, cl, params, ch));
        return;
    }
    
} 