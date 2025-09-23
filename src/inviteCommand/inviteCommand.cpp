#include "inviteCommand.hpp"   
#include  "Server.hpp"  

std::vector<int> inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params   )  
{   
          
                ch.inviteUser( cl ,  Server::getInstance().getUser(params["nickname"]) )   ;    
                return  std::vector<int>() ;    
} ;     