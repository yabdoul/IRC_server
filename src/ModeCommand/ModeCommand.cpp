#include "ModeCommand.hpp"   

void   ModeCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)  
{   
    
    if (!cl.getChannel(ch->getName())) {   
        cl.addMsg(serverResponseFactory::getResp(442 ,  cl   , params  )) ;  
    }
                         
    if (params.find("mode") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(324 , cl ,  params )) ;  
    } 
    
    if (!ch->isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482 , cl , params  ))  ;  
    }
    
    try {
        std::string mode = params["mode"];
        std::string modeParam = "";
        
        if (params.find("mode_param") != params.end()) {
            modeParam = params["mode_param"];
        }
        
        ch->setMode(cl, mode, modeParam);
        
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(501  ,  cl  , params  ) ) ;  
    }
    
}
