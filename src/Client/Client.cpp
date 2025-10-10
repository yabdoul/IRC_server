#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>  
#include "Parser.hpp"  
#include <cstdlib>  
#include <sstream>
#include "Channel.hpp"
#include "Server.hpp"
#include <errno.h>
#include <cstring>
#include "commandFactory.hpp"  
#include "Reactor.hpp"

inline std::string SSTR(int x) {
    std::ostringstream oss;
    oss << std::dec << x;
    return oss.str();
}



class ClientException : public std::exception
{
private:
    const std::string _reason;

public:
    ClientException(const std::string &reason) : _reason("[ClientException]: " + reason)
    {
    }
    ~ClientException() throw() {}
    virtual char const *what() const throw()
    {
        return _reason.c_str();
    }
};  


Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass)
    {    
         _client_fd = client_fd ;  
         _Nick =  Nick ;  
         _User = User  ;  
         _Pass = Pass ;
         _state = CONNECTING;
         _disconnected = false;
    }

Client::Client(int client_fd) :_client_fd(client_fd), _state(CONNECTING), _disconnected(false)
{}

Client::~Client()
{   if(_client_fd) close(_client_fd);
}

Client::Client(const Client &other):IEventHandler(other)
{    
     _client_fd = -1; 
     _Nick = other._Nick;   
     _Pass = other._Pass;
     _User = other._User;
     _state = other._state;
     _realName = other._realName;
     // Don't copy _msgQue or _subscribed2Channel - they should be unique per client
}    
  

void Client::rcvMsg(std::string &Msg)   const 
{ 
    if (_client_fd == -1) {
        return;
    }
    
    // Ensure message ends with \r\n for IRC protocol
    if (!Msg.empty() && Msg.substr(Msg.length() >= 2 ? Msg.length() - 2 : 0) != "\r\n") {
        Msg += "\r\n";
    }
    
    // Use the queue system for proper async sending
    // addMsg(Msg);
}  ;   

  std::map<std::string ,  std::string> Client::userData () const   
{
        std::map<std::string ,  std::string> result ;   
        result["_client_fd"] = SSTR(_client_fd).c_str()      ;    
        result["nick"]   = _Nick ;    
        result["user"] =  _User ;    
        result["server"] = "ascasc" ;   
        return  result  ;   
}  ;     
 

void Client::subscribe2channel(Channel &ch )  
{   
    for(std::vector<Channel * >::iterator it = _subscribed2Channel.begin(); it != _subscribed2Channel.end(); ++it) {
        if( (*it)->getName( ) == ch.getName()) {
            throw std::runtime_error("[Already in The Channel]");
        }
    }
    _subscribed2Channel.push_back(&ch);
} ;   

void  Client::userCommand(Command  & cmd  , std::map<std::string ,  std::string >&params   )  
{ 
    try{  
         cmd.execute(*this ,   params );  
    }  
    catch(std::exception &e  ) 
    { 
         std::cerr<<e.what()<<std::endl ;    
    }
}  ;   
   Channel*  Client::getChannel(std::string chName)  
{     
     for(std::vector<Channel * >::iterator it = _subscribed2Channel.begin()  ;  it != _subscribed2Channel.end() ; it++)   
    {   
       if( (*it)->getName() == chName)     
        return  *it ;      
    }       
    return NULL ;   
}   

void Client::setNickName(std::string &   nick  ) 
 { 
    _Nick = nick ;        
 }
void Client::addMsg(std::string msg) {   
    if (msg.length() < 2 || msg.compare(msg.length() - 2, 2, "\r\n") != 0) {
    msg += "\r\n";
} 
    _msgQue.push_back(msg);   
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = getClientFd();
    Reactor::getInstance().registre(ev, this)   ;
}

void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0');    
        ssize_t n = recv(_client_fd, (void *)buffer.data(), buffer.size(), 0);      
        if (n > 0) {  
            _messageBuffer.append(buffer.data(), n);   
            size_t pos;
            // First try \r\n, then fall back to \n
            while ((pos = _messageBuffer.find("\r\n")) != std::string::npos) {
                std::string command = _messageBuffer.substr(0, pos);
                _messageBuffer.erase(0, pos + 2);
                
                // Parse command and get results immediately to avoid singleton overwrites
                if (Parser::getInstance().parse(command)) {
                    std::string cmd = Parser::getInstance().getCommand();
                    std::map<std::string, std::string> params = Parser::getInstance().getParams();
                    Server::getInstance().callCommand(cmd, params, *this);
                }
            }
            
            // Handle plain \n (Unix line endings)
            while ((pos = _messageBuffer.find("\n")) != std::string::npos) {
                std::string command = _messageBuffer.substr(0, pos);
                _messageBuffer.erase(0, pos + 1);
                
                // Parse command and get results immediately to avoid singleton overwrites
                if (Parser::getInstance().parse(command)) {
                    std::string cmd = Parser::getInstance().getCommand();
                    std::map<std::string, std::string> params = Parser::getInstance().getParams();
                    Server::getInstance().callCommand(cmd, params, *this);
                }
            }
        } else if (n == 0) {
            // Client disconnected normally
            close(_client_fd);
            _client_fd = -1;
            _disconnected = true;
            return;
        } else {
            // Error occurred
            close(_client_fd);
            _client_fd = -1;
            _disconnected = true;
            return;
        }
    } 
      
    if ((e.events & EPOLLOUT) && !_msgQue.empty()) {       
        for(std::vector<std::string>::iterator it = _msgQue.begin(); it != _msgQue.end(); )  
        {   
            std::cout<<this->_client_fd<<"received"<<*it<<std::endl  ;   
            ssize_t bytes_sent = send(_client_fd, it->c_str(), it->size(), MSG_DONTWAIT);
            if (bytes_sent < 0) {
                break;
            } else if (bytes_sent < static_cast<ssize_t>(it->size())) {
                *it = it->substr(bytes_sent);
                break;
            } else {
                it = _msgQue.erase(it);
            }
        }
        
        if (_msgQue.empty()) {
            struct epoll_event ev;
            ev.events = EPOLLIN;  // Only listen for input
            ev.data.fd = getClientFd();
            Reactor::getInstance().registre(ev, this);
        }
    }
}

// Authentication
bool Client::isRegistered() const {
    return _state == REGISTERED;
}

Client::ClientState Client::getState() const {
    return _state;
}

void Client::setState(ClientState state) {
    _state = state;
}

const std::string& Client::getNickname() const {
    return _Nick;
}

const std::string& Client::getUsername() const {
    return _User;
}

const std::string& Client::getRealName() const {
    return _realName;
}

int Client::getClientFd() const {
    return _client_fd;
}

void Client::setUser(const std::string& user) {
    _User = user;
}

void Client::setAuthenticated(bool authenticated) {
    if (authenticated) {
        _state = PASSWORD_SET;
    }
}
