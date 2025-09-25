#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/

void JoinCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)     
{            
    (void )params ;    
    try {  
        if (ch.isUserInChannel(cl)) {
            return ;
        }
        
        if (ch.isInviteOnly() && !ch.isUserInvited(cl)) {    
            cl.addMsg(serverResponseFactory::getResp(473 ,  cl )) ;   
        }
        
        ch.enterChannel(cl);       
        
        cl.addMsg(serverResponseFactory::getResp(353 ,  cl )) ;   
        cl.addMsg(serverResponseFactory::getResp(366 ,  cl )) ;   
    
        // If channel has topic, send it
        if (!ch.getTopic().empty()) {
        cl.addMsg(serverResponseFactory::getResp(332 ,  cl )) ;   
        }
        
    } catch (std::exception &e) { 
              cl.addMsg(serverResponseFactory::getResp(403 ,  cl )) ;   

    }
    
} 