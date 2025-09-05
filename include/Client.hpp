#include "IEventHandler.hpp"
#include <iostream>
#include "IOstream.hpp"
#include <sys/socket.h>
#include <vector>
#include "Iclient.hpp"  

#pragma once 

class Client : public IEventHandler  ,  Iclient   {
private:
    IOstream* _IO;
    int   _client_fd;
    std::string _Nick;
    std::string _Pass;
    std::string _User;
public:
    ~Client();       
    Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO);
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
    void rcvMsg(std::string&  Msg)  const  ;   
};