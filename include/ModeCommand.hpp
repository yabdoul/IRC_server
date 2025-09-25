#include "Command.hpp"  
#include "channelCommand.hpp" 
#include "severResponsFactory.hpp"
#pragma once 
 
class ModeCommand : public Command, public ChannelCommand  
{  
    private:  
    void  execute(Client & sender, std::map<std::string, std::string> &params) {  
            (void) sender, (void) params;     
        }        
    public: 
        ModeCommand() {}; 
        ~ModeCommand() {}; 
       void   exeChannel(Client &cl, Channel &ch, std::map<std::string, std::string> &params = g_emptyMap);   
};
