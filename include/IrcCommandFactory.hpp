#include "Command.hpp"  
#include "JoinCommand.hpp"  
#include "KickCommand.hpp"  

/**
 * @class IrcCommandFactory
 * @brief Factory class for creating IRC command objects.
 *
 * This class provides a centralized way to create various IRC command objects,
 * such as Join and Kick commands. It follows the singleton design pattern to
 * ensure only one instance of the factory exists.
 *
 * @note The constructor is private to enforce the singleton pattern.
 */
class  IrcCommandFactory
{  
    private :  
        IrcCommandFactory()  ;   
    public :   
        ~IrcCommandFactory()    ;   
        static IrcCommandFactory  getInstance() ;   
        Command * createJoinCmd() ;   
        Command * createKickCmd() ;   
} ;  