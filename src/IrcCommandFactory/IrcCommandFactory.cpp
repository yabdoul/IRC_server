#include "IrcCommandFactory.hpp" 


/**
 * @brief Creates a new instance of the JoinCommand.
 * 
 * This function dynamically allocates a new JoinCommand object and returns
 * a pointer to it. The JoinCommand is used to handle the JOIN command in
 * the IRC server, which allows a user to join a specific channel.
 * 
 * @return Command* Pointer to the newly created JoinCommand object.
 */
Command  *  IrcCommandFactory::createJoinCmd() 
{  
    Command  *   JoinCmd  =  new JoinCommand() ;  
    return JoinCmd ;     
}

/**
 * @brief Creates a new KickCommand instance.
 * 
 * This function dynamically allocates a new KickCommand object and returns 
 * a pointer to it. The KickCommand is used to handle the KICK command in IRC, 
 * which allows a user to remove another user from a channel.
 * 
 * @return Command* Pointer to the newly created KickCommand instance.
 */
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