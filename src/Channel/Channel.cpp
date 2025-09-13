#include  "Channel.hpp"
#include "channelCommand.hpp"  

Channel::Channel(){  
      _invitOnly = false  ;  
}  
Channel::~Channel(){}   
// Channel::Channel(Channel &copy){ _inviteList =  copy._inviteList ;}    

Channel& Channel::operator=(Channel &c )  
{  
      _inviteList  = c._inviteList  ;  
      return *this ;   
}     

void Channel::ExecuteCommand(Command * cmd  ,  Client client   ,  std::map<std::string  , std::string>params  )     
{     
      
      /* 
                     NEXT:   implement a  logic to check if the class is derived from  base Channelcommand , and then call the propper methode  !  , then call the other one  !  ps : the some commands inherits the two classes but some not 
                        Pass the Propper Params for each one , then test  some tests 
      */ 
      if(dynamic_cast<ChannelCommand *> (cmd) )  
      { 
            ChannelCommand * tmp =  dynamic_cast<ChannelCommand *>  (cmd) ;    
            tmp->exeChannel() ;   
      }   
      else {  
            cmd->execute() ;    
      }
}  ;   