#include <map>  
#include "Command.hpp"    
#include <algorithm>  
#include <algorithm>  
#include <exception>
#include <ctime>          
#ifndef   CHANNEL_HPP  
#define CHANNEL_HPP 
#pragma once    
class Client ;    
class Command ;   
class Channel  {  
        private :  
                 std::map<Client*, int>    _inviteList ;    
                 std::string _channelName ;            
                 bool _invitOnly ;
                 bool _topicRestricted;
                 std::string _topic;
                 std::string _topicSetter;      // Who set the topic
                 time_t _topicTimestamp;        // When topic was set
                 std::string _key;  // Channel password
                 int _userLimit;    // 0 = no limit     
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
                std::vector<Client  *>  getUsers() ;    
                void rcvMsg(std::string  &msg  )  ; 
                void lockChannel(Client & sender )  ;         
                void ExecuteCommand(Command  &  cmd  , Client&  Client  , std::map<std::string , std::string>  params  )     ;     
                void inviteUser(Client &sender ,  Client &target )  ;      
                void  enterChannel(Client &cl  ) ;
                void kickUser(Client &sender, Client &target, const std::string& reason);
                void removeUser(Client &client);
                void setTopic(Client &sender, const std::string& newTopic);
                const std::string& getTopic() const;
                const std::string& getTopicSetter() const;
                time_t getTopicTimestamp() const;
                void setMode(Client &sender, const std::string& mode, const std::string& param);
                void broadcastMessage(const std::string& message, Client* exclude = NULL);
                bool isUserInChannel(const Client& user) const;
                bool isInviteOnly() const;
                bool isTopicRestricted() const;
                bool isUserInvited(const Client& user) const;
                std::vector<Client*> getChannelMembers() const;
                const std::string& getName() const;   
} ;    
#endif  