#include "KickCommand.hpp"  
#include "Server.hpp"

KickCommand::KickCommand() 
{ 
     
}

void KickCommand::exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params)
{
    std::vector<int> responses;  
    if (params.find("nickname") == params.end()) {
        cl.addMsg(serverResponseFactory::getResp(401 ,  cl )) ;    
    }
    try {
        if (!ch.isUserInChannel(cl)) {
            cl.addMsg(serverResponseFactory::getResp(442 ,   cl ));   
        }
        
        if (!ch.isOp(cl)) {
            cl.addMsg(serverResponseFactory::getResp(482 ,  cl )) ;    
        }
         
        Client& targetClient = Server::getInstance().getUser(params["nickname"]);
        
        if (!ch.isUserInChannel(targetClient)) {
            cl.addMsg(serverResponseFactory::getResp(441 ,  cl )) ;    

        }
        
        std::string reason = "No reason given";
        if (params.find("reason") != params.end()) {
            reason = params["reason"];
        }
        ch.kickUser(cl, targetClient, reason);
    } catch (std::exception& e) {
        cl.addMsg(serverResponseFactory::getResp(401 ,  cl )) ;    
    }
    
}  
