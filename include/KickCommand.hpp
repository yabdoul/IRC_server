#include "Command.hpp" 
#include "channelCommand.hpp"
#include "severResponsFactory.hpp"  

class KickCommand :  public  Command, public ChannelCommand  
 {    
    private : 
      void execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )  {  
                (void) sender  , (void ) params ;}  
    public : 
        KickCommand();   
        ~KickCommand(){} ;  
    void  exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params = g_emptyMap);
};  