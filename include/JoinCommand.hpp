#include "Command.hpp"  
#include "channelCommand.hpp"
class JoinCommand :  public Command  , public ChannelCommand   
{   
    public :  
        JoinCommand() {}; 
        ~JoinCommand() {} ;   
        void execute() ;           
        void exeChannel(Client &cl , Channel &ch  )  ;    
};  