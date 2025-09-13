#include "Channel.hpp"  

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

void Channel::ExecuteCommand(Command * cmd  ,  Client client )  
{
      
}