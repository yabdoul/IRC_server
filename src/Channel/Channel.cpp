#include "Channel.hpp"    
 

void  Channel::ChannelEvent(Command   * cmd  ,  Client Client  )  {    
     if(!cmd )  
            std::cerr<<"invalid Command "<<std::endl  ;  
     cmd->execute( ) ;    
}  