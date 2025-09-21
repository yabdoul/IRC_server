#include <map>  
#include "Command.hpp"    
#include <algorithm>  
#include "Ichannel.hpp"  
#include <algorithm>  
#include <exception>          
#ifndef   CHANNEL_HPP  
#define CHANNEL_HPP 
#pragma once    
class Client ;   
class Channel  {  
        private :  
                 std::map<Client  , int   >    _inviteList ;    
                 std::string _channelName ;            
                 bool _invitOnly ;     
        public :   
                Channel() ;  
                Channel(std::string channelName  ,  Client & owner)  ;     
                ~Channel();         
                Channel& operator=(Channel &c )    ;     
                bool  operator==(const  Channel &other ) const 
                {    
                    return((_channelName == other._channelName)?(true):(false)) ;   
                }
                bool isOp(Client &sender )   ;      
                void rcvMsg(std::string  &msg  )  ; 
                void lockChannel(Client & sender )  ;        
                void ExecuteCommand(Command  *  cmd  , Client Client  , std::map<std::string , std::string>  params  )   ;     
                void inviteUser(Client &sender ,  Client &target )  ;      
                void  enterChannel(Client &cl  ) ;       
                // void handleTopic(Client& sender, const std::string& newTopic = "");
                // void handleMode(Client& sender , const std::string& mode, const std::string& param = "") ; 
                // void IsSubscribed(Client &C  )  const    ;   
} ;    
#endif  