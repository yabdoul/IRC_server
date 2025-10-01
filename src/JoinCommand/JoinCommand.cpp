#include "JoinCommand.hpp"
  
/* 
    Implement an Execute Command in  Channel Class 
*/

void JoinCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)     
{            
    (void )params ;           
    std::cerr<<"entred join command scoop"<<std::endl ;   
    try {    

        if (ch.isInviteOnly() && !ch.isUserInvited(cl)) {       
            cl.addMsg(serverResponseFactory::getResp(473 ,  cl   , params  ,  &ch    )) ;   
        }
        
        ch.enterChannel(&cl);       
        cl.addMsg(serverResponseFactory::getResp(353 ,  cl  , params  ,  &ch   )) ;   
        cl.addMsg(serverResponseFactory::getResp(366 ,  cl   , params  ,  &ch    )) ;  
        if (!ch.getTopic().empty()) {
        cl.addMsg(serverResponseFactory::getResp(332 ,  cl  , params   ,  &ch   )) ;   
        }
        
    } catch (std::exception &e) { 
              cl.addMsg(serverResponseFactory::getResp(403 ,  cl  , params   , &ch   )) ;   

    }
    
} 