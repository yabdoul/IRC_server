#include "Channel.hpp"  

Channel::Channel(){}  
Channel::~Channel(){}   
// Channel::Channel(Channel &copy){ _inviteList =  copy._inviteList ;}    


Channel& Channel::operator=(Channel &c )  
{  
      _inviteList  = c._inviteList  ;  
      return *this ;   
}  

void Channel::handleKick(Client& sender,  std::string& targetNick, const std::string& reason)  
{        
      (void ) reason ;    
      if(_inviteList[sender] ==  "OP" )
      {   
            _inviteList.erase(Client(targetNick)) ;     
      }
      else 
            std::cout<<"NO Privilege"  ;       

}       
void  Channel::handleJoin(Client &sender  )  
{     
      if(_inviteList.count(sender) !=  0 )  
      { 
             _online.push_back(sender)  ;   
             /* 
             [TOOD]:       
                  Response , Join  Command echo Goes Here !   
             */
      }  
      else  
            throw std::runtime_error("[JOIN] CLIENT not in invite List" ) ;    
}

void Channel::rcvMsg(std::string &msg )  
{ 
        std::map<Client , std::string>::iterator it  ;   
        for(it  =   _inviteList.begin () ;  it  !=  _inviteList.end() ; it++ )  
                  (it)->first.rcvMsg(msg) ;    
}   
   

void Channel::handleInvite(Client  & sender ,  Client &target )  
{ 
      if(_inviteList[sender]  == "OP")
      {  
         if(_inviteList.count(target)   != 0  )  
         {   throw std::runtime_error("[INVITE] Already HERE ! "  ) ;    }
            _online.push_back(target)  ;   
            /* 
            [TODO invite Response]      
            */
      } 
      else  {    
            /*
                  ServerResponse if the sender itsnt operator ;   
            */
            throw  std::runtime_error("[INVITE] not an Operator ERROR") ;    
      
      } 
}  ;   