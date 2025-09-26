#include "TopicCommand.hpp"   
#include "Server.hpp"

void  TopicCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)  
{   
    // Check if user is in the channel
    if (!ch.isUserInChannel(cl)) {  
        cl.addMsg(serverResponseFactory::getResp(442 ,  cl , params   ));  // ERR_NOTONCHANNEL
        return;
    }
    
    // If no topic parameter provided, return current topic
    if (params.find("topic") == params.end()) {
        if (ch.getTopic().empty()) {
            cl.addMsg(serverResponseFactory::getResp(331  ,  cl  ,  params  ));  // RPL_NOTOPIC
        } else {
            cl.addMsg(serverResponseFactory::getResp(332  ,  cl  ,  params   ));  // RPL_TOPIC
            // Send topic setter info if topic exists and has been set by someone
            if (ch.getTopicTimestamp() > 0) {
                cl.addMsg(serverResponseFactory::getResp(333  ,  cl  ,  params  ));  // RPL_TOPICWHOTIME
            }
        }
        return;
    }
    
    // Check if topic is restricted and user is not operator
    if (ch.isTopicRestricted() && !ch.isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl , params ));  // ERR_CHANOPRIVSNEEDED
        return;
    }
    
    try {
        ch.setTopic(cl, params["topic"]);
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(482  ,  cl , params   ));  // ERR_CHANOPRIVSNEEDED
    }
}
