#include "Command.hpp" 
#include "channelCommand.hpp"
 
class KickCommand : public Command, public ChannelCommand  
{    
    private : 
        std::vector<int> execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )  {  
                (void) sender  , (void ) params ;     
                return std::vector<int>() ;  
        }  
    public : 
        KickCommand();   
        ~KickCommand(){} ;  
        std::vector<int> exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params = g_emptyMap);
};  