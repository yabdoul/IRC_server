#include "Command.hpp"  
#include "JoinCommand.hpp"  
#include "KickCommand.hpp"  

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