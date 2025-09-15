#include "inviteCommand.hpp"   
#include  "Server.hpp"  

void  inviteCommand::execute() { 
      throw std::runtime_error("[INVITE] : is a Channel Command") ;   
}   
  

void  inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params   )  
{   
          
                ch.inviteUser( cl ,  Server::getInstance().getUser(params["nickname"]) )   ;  
} ;     