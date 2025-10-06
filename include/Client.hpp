#include "IEventHandler.hpp"
#include <iostream>
#include <sys/socket.h>
#include <vector> 
#include "Command.hpp" 
#include "Iclient.hpp"    
#include "Channel.hpp" 
#include <map>  
#pragma once 
#ifndef CLIENT_HPP 
#define CLIENT_HPP
class Command;

class Client : public IEventHandler  ,  Iclient   {
public:
    enum ClientState {
        CONNECTING,    
        PASSWORD_SET,  
        NICK_SET,     
        REGISTERED     
    };
    
private:
    int   _client_fd;
    std::string _Nick;
    std::string _Pass;
    std::string _User;
    std::string _realName;
    std::string _hostname;
    ClientState _state;  
    std::vector<std::string> _msgQue ;  
    std::string _messageBuffer;
    std::vector<Channel  *  >  _subscribed2Channel ;  
public:
    ~Client();       
    Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass );
    Client(int client_fd);  
    Client(std::string Nick ):_Nick(Nick){} ;    
    Client(const Client &other)  ;   
 
    bool operator<(const   Client & other   )   const  
    { 
        return  _Nick > other._Nick ;  
    }
    Client& operator=(const Client& other)    {   
        _client_fd =  other._client_fd ;   
        return *this;   
    } ; 
    bool operator==(const  Client & C)  const  
    {  
        return( (C._Nick ==   _Nick )?(true):(false)) ;    
    } ;     
    virtual void handle_event(epoll_event   ev);      
     std::map<std::string ,  std::string> userData() const  ;   
    void rcvMsg(std::string&  Msg)  const  ;     
    void subscribe2channel(Channel &ch  )   ;  
     Channel *  getChannel(std::string chName ) ;      
    void userCommand(Command &  cmd ,std::map<std::string ,  std::string >&params  )  ;
    bool isRegistered() const;
    ClientState getState() const;
    void setState(ClientState state);
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealName() const  ; 
    void addMsg(std::string msg);   
    int getClientFd() const;   
    void setNickName(std::string&  nick ) ;     
    std::string  getNickName( )  const   
    {  
        return _Nick ;   
    }
};

#endif