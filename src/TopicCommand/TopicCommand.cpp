#include "TopicCommand.hpp"   
#include "Server.hpp"

void  TopicCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)  
{   
    
    if (!cl.getChannel(ch->getName())) {
        cl.addMsg(serverResponseFactory::getResp(442 ,  cl , params   ));  
        return;
    }
    
    
    if (params.find("topic") == params.end()) {
        if (ch->getTopic().empty()) {

            cl.addMsg(serverResponseFactory::getResp(331  ,  cl  ,  params  ));  
        } else {
            cl.addMsg(serverResponseFactory::getResp(332  ,  cl  ,  params   ));  
            
            if (ch->getTopicTimestamp() > 0) {
                cl.addMsg(serverResponseFactory::getResp(333  ,  cl  ,  params  ));  
            }
        }
        return;
    }
    
    
    if (ch->isTopicRestricted() && !ch->isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl , params ));  
        return;
    }
    
    try {
        ch->setTopic(cl, params["topic"]);
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl , params   ));  
    }
}
