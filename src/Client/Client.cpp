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
    }

Client::Client(int client_fd) :_client_fd(client_fd), _state(CONNECTING)
{}

Client::~Client()
{   if(_client_fd) close(_client_fd);
}

Client::Client(const Client &other):IEventHandler(other)
{    
    std::cout<<"entre copy"<<std::endl ;   
     _client_fd = -1; 
     _Nick = other._Nick;   
     _Pass = other._Pass;
     _User = other._User;
     _state = other._state;
     _realName = other._realName;
     // Don't copy _msgQue or _subscribed2Channel - they should be unique per client
}  

void Client::rcvMsg(std::string &Msg) const  
{ 

    if (_client_fd == -1) {
        return;
    }
    
    ssize_t bytes_sent = send(_client_fd, Msg.c_str(), Msg.size(), MSG_DONTWAIT);
    
    if (bytes_sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cout << "Send buffer full for client " << _Nick << std::endl;
        } else {
            std::cout << "Send error for client " << _Nick << ": " << strerror(errno) << std::endl;
        }
    } else if (bytes_sent < static_cast<ssize_t>(Msg.size())) {
        std::cout << "Partial send for client " << _Nick << ": " << bytes_sent << "/" << Msg.size() << " bytes" << std::endl;
    }
}  ;   

  std::map<std::string ,  std::string> Client::userData () const   
{
        std::map<std::string ,  std::string> result ;   
        result["_client_fd"] = SSTR(_client_fd).c_str()      ;    
        result["nickname"]   = _Nick ;    
        result["user"] =  _User ;    
        result["server"] = "ascasc" ;   
        return  result  ;   
}  ;     
 

void Client::subscribe2channel(Channel &ch )  
{   
    // Check if channel is already subscribed by comparing names
    for(std::vector<Channel>::iterator it = _subscribed2Channel.begin(); it != _subscribed2Channel.end(); ++it) {
        if(it->getName() == ch.getName()) {
            throw std::runtime_error("[Already in The Channel]");
        }
    }
    _subscribed2Channel.push_back(ch);
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
 Channel  Client::getChannel(std::string chName)  
{   
     for(std::vector<Channel>::const_iterator it = _subscribed2Channel.begin()  ;  it != _subscribed2Channel.end() ; it++)   
    { 
       if(it->getName() == chName)  
        return *it ;      
    }       
    return(Server::getInstance().AddChannel(chName));   
}   

void Client::addMsg(std::string msg) {  
    _msgQue.push_back(msg);
    // Register for EPOLLOUT when we have messages to send
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = getClientFd();
    Reactor::getInstance().registre(ev, this);
    
    std::cout << "Added message to queue for client " << _Nick << ", registered EPOLLOUT" << std::endl;
}

void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0');  
        ssize_t n = recv(_client_fd, (void *)buffer.data(), buffer.size(), 0);
        
        if (n > 0) {  
            std::cout << "RAW string: " << std::string(buffer.data(), n) << std::endl;
            _messageBuffer.append(buffer.data(), n);
            size_t pos;
            while ((pos = _messageBuffer.find("\r\n")) != std::string::npos) {
                std::string command = _messageBuffer.substr(0, pos);
                _messageBuffer.erase(0, pos + 2);
                Parser::getInstance().parse(command) ;  
                Command * Cmd  =  commandFactory::makeCommand(Parser::getInstance().getCommand())  ;  
                if(Cmd) {
                    try {
                        if(dynamic_cast<ChannelCommand  *> (Cmd) )  
                        {   
                            Channel target = getChannel(Parser::getInstance().getParams().at("channel")) ;    
                            target.ExecuteCommand(*Cmd ,  *this , Parser::getInstance().getParams()) ;       
                        }  
                        else { 
                            std::map<std::string, std::string> params = Parser::getInstance().getParams();
                            userCommand(*Cmd, params);
                        }
                        Server::getInstance().beReady2Send() ;   
                            
                    } catch(const std::exception& e) {
                        std::cerr << "Command execution error: " << e.what() << std::endl;
                    }   
                } 

            }
        } else if (n == 0) {
            std::cout << "Client " << _Nick << " disconnected" << std::endl;
            close(_client_fd);
            _client_fd = -1;
            return;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            } else {
                std::cerr << "recv() error for client " << _Nick << ": " << strerror(errno) << std::endl;
                close(_client_fd);
                _client_fd = -1;
                return;
            }
        }
    } 
    
    if ((e.events & EPOLLOUT) && !_msgQue.empty()) {    
        std::cout << "Handling EPOLLOUT event for client " << _Nick << std::endl;   
        for(std::vector<std::string>::iterator it = _msgQue.begin(); it != _msgQue.end(); )  
        { 
            ssize_t bytes_sent = send(_client_fd, it->c_str(), it->size(), MSG_DONTWAIT);
            if (bytes_sent < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    std::cout << "Send would block, will retry later" << std::endl;
                    break;
                } else {
                    std::cerr << "Send error for client " << _Nick << ": " << strerror(errno) << std::endl;
                    break;
                }
            } else if (bytes_sent < static_cast<ssize_t>(it->size())) {
                // Partial send, update message and retry later
                *it = it->substr(bytes_sent);
                std::cout << "Partial send: " << bytes_sent << " bytes, remaining: " << it->size() << std::endl;
                break;
            } else {
                // Complete send, remove message from queue
                std::cout << "Message sent successfully: " << bytes_sent << " bytes" << std::endl;
                it = _msgQue.erase(it);
            }
        }
        
        // If queue is empty, remove EPOLLOUT to avoid unnecessary wake-ups
        if (_msgQue.empty()) {
            struct epoll_event ev;
            ev.events = EPOLLIN;  // Only listen for input
            ev.data.fd = getClientFd();
            Reactor::getInstance().registre(ev, this);
            std::cout << "Message queue empty, removed EPOLLOUT for client " << _Nick << std::endl;
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
