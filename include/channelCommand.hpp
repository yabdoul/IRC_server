#include "Client.hpp"  
#include  "Channel.hpp"  
#include <map>     
#pragma once 
static std::map<std::string, std::string> g_emptyMap;   

class ChannelCommand{   
    public :   
        ChannelCommand() ;  
        ~ChannelCommand() ;   
        virtual void exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string>&params  =  g_emptyMap ) =0  ;   
} ;  