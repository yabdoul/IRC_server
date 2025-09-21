#include  "Channel.hpp"
#include "channelCommand.hpp"  
#include <algorithm>  
#include "Client.hpp"  
enum Roles {
    OP,
    INV,
    MEMB
};

Channel::Channel(){  
      _invitOnly = false  ;  
}  
Channel::~Channel(){}   
// Channel::Channel(Channel &copy){ _inviteList =  copy._inviteList ;}    
Channel::Channel(std::string channelName  ,  Client & owner):_channelName(channelName)
{ 
      _inviteList[owner] = OP  ;   
}


void Channel::ExecuteCommand(Command * cmd  ,  Client client   ,  std::map<std::string  , std::string>params  )     
{       
      (void) params ;   
      if(dynamic_cast<ChannelCommand *> (cmd) )  
      { 
            ChannelCommand * tmp =  dynamic_cast<ChannelCommand *>  (cmd) ;    
            tmp->exeChannel(client ,  *this , params) ;   
      }   
      else {  
            cmd->execute() ;    
      }
}  ;   

bool Channel::isOp(Client & sender )  {
     return((_inviteList[sender] ==OP)?(true):(false)) ;}  

void Channel::inviteUser(Client &sender ,  Client  &target )    
{    
      if(isOp(sender))  
      { 
            if(_inviteList.count(target) != 0 )  
                  throw std::runtime_error("[Join]:User Already in Invite List") ;    
            _inviteList[target] =  INV ;      
            return ;   
      }           
      throw std::runtime_error("[JOIN] : You are  Not An OPP ") ;   
}  ;   
 
void Channel::lockChannel(Client &sender)  
{ 
      _invitOnly = ((isOp(sender))?true:false)  ;  
}
void Channel::enterChannel(Client &cl  ) 
{    
      if(_inviteList.count(cl) == 0 )   
            throw std::runtime_error("[JOIN]:User Not in Guest List Ask OP ") ;    
      if(!_inviteList.count(cl))   
          _inviteList[cl] =  MEMB ; 
      else 
            throw std::runtime_error("[JOIN] : ALREADY HERE" );   
}