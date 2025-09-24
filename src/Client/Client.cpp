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
     _Nick =  other._Nick ;   
     _Pass = other._Pass ;     
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
    if(std::find(_subscribed2Channel.begin() ,  _subscribed2Channel.end()  ,  &ch) == _subscribed2Channel.end() )
      _subscribed2Channel.push_back(&ch) ;    
    else 
        throw std::runtime_error("[Already in The Channel]") ;   
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
    
    } else if (e.events & EPOLLOUT) {
        // TODO: Implement send buffer handling for better performance
        // For now, basic implementation
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
