#include "IEventHandler.hpp"
#include <iostream>
#include "IOstream.hpp"
#include <sys/socket.h>
#include <vector>
#include "Command.hpp"  
#include "Iclient.hpp"   

#include <map>  
#ifndef   CLIENT_HPP  
#define CLINET_HPP  
#pragma once 

class Channel ;     
 
class Client : public IEventHandler  ,  Iclient   {
public:
    enum ClientState {
        CONNECTING,     // Connected, needs PASS
        PASSWORD_SET,   // PASS, needs NICK
        NICK_SET,       // NICK, needs USER
        REGISTERED      // Fully authenticated
    };
    
private:
    int   _client_fd;
    std::string _Nick;
    std::string _Pass;
    std::string _User;
    std::string _realName;
    std::string _hostname;
    ClientState _state;
    std::string _messageBuffer;  // For partial message case
    std::vector<Channel *  >  _subscribed2Channel ;  
public:
    ~Client();       
    Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass );
    Client(int client_fd);  
    Client(std::string Nick ):_Nick(Nick){} ;    
    Client(const Client &other)  ;   
    // Client() ;    
 
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
    /* 
        Parser Should Send a map of Params any way ;    
    */     
    void userCommand(Command *  cmd , std::vector<std::string>  params  )  ;
    
    bool isRegistered() const;
    ClientState getState() const;
    void setState(ClientState state);
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealName() const;
    int getClientFd() const;
    
    void processCommand(const std::string& command);
    void handlePassCommand(const std::string& password);
    void handleNickCommand(const std::string& nickname);
    void handleUserCommand(const std::string& username, const std::string& hostname, 
                          const std::string& servername, const std::string& realname);
    void sendWelcomeSequence();
    void handlePrivmsgCommand(const std::map<std::string, std::string>& params);
    void handleJoinCommand(const std::map<std::string, std::string>& params);
    void handleKickCommand(const std::map<std::string, std::string>& params);
    void handleInviteCommand(const std::map<std::string, std::string>& params);
    void handleTopicCommand(const std::map<std::string, std::string>& params);
    void handleModeCommand(const std::map<std::string, std::string>& params);
    void handlePartCommand(const std::map<std::string, std::string>& params);    
};

#endif