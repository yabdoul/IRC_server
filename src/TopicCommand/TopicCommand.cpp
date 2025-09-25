#include "TopicCommand.hpp"   

void  TopicCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)  
{   
    if (!ch.isUserInChannel(cl)) {  
        cl.addMsg(serverResponseFactory::getResp(442 ,  cl ) )  ;    
    }
    if (params.find("topic") == params.end()) {
        if (ch.getTopic().empty()) {
            cl.addMsg(serverResponseFactory::getResp(331  ,  cl ) )  ;    
        } else {
            cl.addMsg(serverResponseFactory::getResp(332  ,  cl ) )  ;    
        }
    }
    
    if (ch.isTopicRestricted() && !ch.isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl ) )  ;    
    }
    
    try {
        ch.setTopic(cl, params["topic"]);
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl ) )  ;    

    }
}
