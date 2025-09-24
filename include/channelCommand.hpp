#include "Client.hpp"  
#include  "Channel.hpp"  
#include <map>  
#include <vector>  

#pragma once  
static std::map<std::string, std::string> g_emptyMap;   

class ChannelCommand{   
    public :   
        ChannelCommand() ;  
        ~ChannelCommand() ;   
        virtual std::vector<int>  exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string>&params  =  g_emptyMap ) =0  ;   
} ;  