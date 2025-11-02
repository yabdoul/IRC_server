#include "PartCommand.hpp"
#include "Server.hpp"

void PartCommand::exeChannel(Client& cl, Channel *   ch, std::map<std::string, std::string>& params)
{
    try {
        if (!cl.getChannel(ch->getName())) {
            
            cl.addMsg(serverResponseFactory::getResp(442, cl , params  ));
            return;
        }
        
        
        std::string partMessage = "Leaving";
        if (params.find("message") != params.end()) {
            partMessage = params["message"];
        }
        
        std::string partMsg = ":" + cl.getNickname() + "!" + cl.getUsername() + 
                              "@localhost PART #" + ch->getName() + " :" + partMessage + "\r\n";
        
        ch->broadcastMessage(cl ,  partMsg );
        
        ch->removeUser(cl);
        
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(403, cl , params ));
    }
}
