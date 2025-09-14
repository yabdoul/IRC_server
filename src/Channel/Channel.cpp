#include  "Channel.hpp"
#include "channelCommand.hpp"  
#include <algorithm>  

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
      (void) params ;   
      
      /* 
                     NEXT:   implement a  logic to check if the class is derived from  base Channelcommand , and then call the propper methode  !  , then call the other one  !  ps : the some commands inherits the two classes but some not 
                        Pass the Propper Params for each one , then test  some tests 
      */ 
      if(dynamic_cast<ChannelCommand *> (cmd) )  
      { 
            ChannelCommand * tmp =  dynamic_cast<ChannelCommand *>  (cmd) ;    
            tmp->exeChannel(client ,  *this) ;   
      }   
      else {  
            cmd->execute() ;    
      }
}  ;   

bool Channel::isOp(Client & sender )  {
     return((_inviteList[sender] == "OP")?(true):(false)) ;}  

void Channel::inviteUser(Client &sender ,  Client  &target )    
{    
      if(isOp(sender))  
      { 
            if(_inviteList.count(target) != 0 )  
                  throw std::runtime_error("[Join]:User Already in Invite List") ;    
            _inviteList[target] =  "INV" ;      
            return ;   
      }           
      throw std::runtime_error("[JOIN] : You are  Not An OPP ") ;   
}  ;   
 
void Channel::enterChannel(Client &cl  ) 
{    
      if(_inviteList.count(cl) == 0 )   
            throw std::runtime_error("[JOIN]:User Not in Guest List Ask OP ") ;    
      if(std::find(_online.begin() ,  _online.end() ,  cl) ==   _online.end( ) )  
            _online.push_back(cl) ;    
      else 
            throw std::runtime_error("[JOIN] : ALREADY HERE" );   
}