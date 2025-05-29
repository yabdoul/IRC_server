#include "IrcCommandFactory.hpp" 

 
IrcCommandFactory IrcCommandFactory::getInstance() 
{  
        static IrcCommandFactory instance  ; 
        return instance ;  
}  

Command  *  IrcCommandFactory::createJoinCmd() 
{  
    Command  *   JoinCmd  =  new JoinCommand() ;  
    return JoinCmd ;     
}
 
Command *  IrcCommandFactory::createKickCmd()  
{  
    Command * KickCmd =  new KickCommand() ;  
    return KickCmd ;  
} 

