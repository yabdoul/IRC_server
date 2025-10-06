#include "Command.hpp"
#include "severResponsFactory.hpp"
#include "channelCommand.hpp"  

#pragma once

class PrivmsgCommand : public Command  ,  public  ChannelCommand    
{  
    public:
        void execute(Client& sender, std::map<std::string, std::string>& params);
        void exeChannel(Client &cl , Channel *ch  , std::map<std::string ,  std::string>&params  =  g_emptyMap ) ;    
        PrivmsgCommand();
        ~PrivmsgCommand();
};
