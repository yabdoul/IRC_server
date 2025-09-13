#include "Client.hpp"  
#include  "Channel.hpp"  

class ChannelCommand{   
    public :   
        ChannelCommand() ;  
        ~ChannelCommand() ;   
        virtual void exeChannel(Client &cl , Channel &ch  ) =0  ;   
} ;  