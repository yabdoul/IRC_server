#include "IrcCommandFactory.hpp" 


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

IrcCommandFactory::IrcCommandFactory()
{} 

IrcCommandFactory IrcCommandFactory::getInstance() 
{  
       static IrcCommandFactory instance  ; 
       return instance ;  
}  

IrcCommandFactory::~IrcCommandFactory() 
{ 
     
}