#include  "Command.hpp" 
#include  "channelCommand.hpp"   
#include "severResponsFactory.hpp"

#pragma once 
 
class inviteCommand : public Command  , public ChannelCommand  {  
    private:  
       void   execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )  {  
            (void) sender  , (void ) params ;     
        }        
    public : 
            inviteCommand() {} ;  
            ~inviteCommand(){} ;   
          void exeChannel(Client  &cl ,  Channel &ch   , std::map<std::string ,  std::string >&params  =  g_emptyMap)   ;   
}  ;   