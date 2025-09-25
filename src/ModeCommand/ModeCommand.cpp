#include "ModeCommand.hpp"   

void   ModeCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)  
{   
    
    if (!ch.isUserInChannel(cl)) {   
        cl.addMsg(serverResponseFactory::getResp(442 ,  cl )) ;  
    }
    
    if (params.find("mode") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(324 , cl )) ;  
    }
    
    if (!ch.isOp(cl)) {
        cl.addMsg(serverResponseFactory::getResp(482 , cl  ))  ;  
    }
    
    try {
        std::string mode = params["mode"];
        std::string modeParam = "";
        
        if (params.find("mode_param") != params.end()) {
            modeParam = params["mode_param"];
        }
        
        ch.setMode(cl, mode, modeParam);
        
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(501  ,  cl ) ) ;  
    }
    
}
