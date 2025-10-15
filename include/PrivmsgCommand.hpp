#include "Command.hpp"
#include "severResponsFactory.hpp"
#include "channelCommand.hpp"  

#pragma once

class PrivmsgCommand :  public  ChannelCommand , public  Command   
{  
    private:  
        void execute(Client& sender, std::map<std::string, std::string>& params){ 
              (void) sender  ;  
              (void )  params ;   
              throw std::runtime_error("Action Not Allowed")   ;   
        }
    public:
        void exeChannel(Client &cl , Channel *ch  , std::map<std::string ,  std::string>&params  =  g_emptyMap ) ;    
        PrivmsgCommand();
        ~PrivmsgCommand();
};
