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
  

void Client::rcvMsg(std::string &Msg)   const 
{ 
    if (_client_fd == -1) {
        std::cout << "Cannot send to client " << _Nick << " - invalid file descriptor" << std::endl;
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
    return(Server::getInstance().AddChannel(chName  ,  *this ));   
}   

void Client::setNickName(std::string &   nick  ) 
 { 
    _Nick = nick ;        
 }
void Client::addMsg(std::string msg) {       
    std::cout<<"addMsg Scoop"<<msg<<std::endl  ;  
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
            while ((pos = _messageBuffer.find("\r\n")) != std::string::npos) {
                std::string command = _messageBuffer.substr(0, pos);
                _messageBuffer.erase(0, pos + 2);
                Parser::getInstance().parse(command) ;  
                Command * Cmd  =  commandFactory::makeCommand(Parser::getInstance().getCommand())  ;     
                if(Cmd) {
                    try {    
                        std::cout<<Parser::getInstance().getCommand()<<std::endl  ;       
                        if(Cmd)  
                        {                         
                            if(dynamic_cast<ChannelCommand  *> (Cmd) )  
                            {   
                                Channel target = getChannel(Parser::getInstance().getParams().at("channel")) ;      
                                std::cout<<"Executing"<<std::endl ;   
                                target.ExecuteCommand(*Cmd ,  *this , Parser::getInstance().getParams()) ;              
                            }    
                            else {  
                                std::cout<<"HERE"<<std::endl ;   
                                std::map<std::string, std::string> params = Parser::getInstance().getParams();
                                userCommand(*Cmd, params);
                            }  
                            Server::getInstance().beReady2Send() ;    
                        }   
                    } catch(const std::exception& e) {
                        std::cerr << "Command execution error: " << e.what() << std::endl;   
                    }   
                    delete Cmd;  
                }   
                /* 
                        =================================================================
                        ==3637==ERROR: AddressSanitizer: stack-use-after-scope on address 0x7fd479302730 at pc 0x7fd47b445f1e bp 0x7ffc8fc78f40 sp 0x7ffc8fc786e8
                        READ of size 6 at 0x7fd479302730 thread T0
                        #0 0x7fd47b445f1d in memcpy (/usr/lib/x86_64-linux-gnu/libasan.so.8+0x100f1d) (BuildId: f1bcae188e96eba85c822b6bdce2858c59963ad1)
                            #1 0x7fd47b2257d5 in void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char*>(char*, char*, std::forward_iterator_tag) (/usr/lib/x86_64-linux-gnu/libstdc++.so.6+0x1777d5) (BuildId: bd00d851857c6423cde21b91def861ceed2c23d7)
                            #2 0x55ddb35b645b in Channel::getName[abi:cxx11]() include/Channel.hpp:51
                            #3 0x55ddb35d0b91 in Server::IsChannelExist(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) src/server/server.cpp:186
                            #4 0x55ddb35d1379 in Server::callCommand(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >&, Client&) src/server/server.cpp:220
                            #5 0x55ddb35b5097 in Client::handle_event(epoll_event) src/Client/Client.cpp:178
                            #6 0x55ddb35ca910 in Reactor::notify(epoll_event) src/reactor/reactor.cpp:125
                            #7 0x55ddb35c8feb in Demultiplexer::Demultiplexer() src/demultiplexer/demultiplexer.cpp:39
                            #8 0x55ddb35c9585 in Reactor::Run() src/reactor/reactor.cpp:44
                            #9 0x55ddb35de549 in main src/test.cpp:33
                */
                Server::getInstance().callCommand(Parser::getInstance().getCommand() ,  Parser::getInstance().getParams() , *this     ) ;   
            }
        } else if (n == 0) {
            close(_client_fd);
            _client_fd = -1;
            return;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            } else {
                close(_client_fd);
                _client_fd = -1;
                return;
            }
        }
    } 
      
    if ((e.events & EPOLLOUT) && !_msgQue.empty()) {       
        std::cout<<"EPOLLOUT"<<std::endl ;   
        for(std::vector<std::string>::iterator it = _msgQue.begin(); it != _msgQue.end(); )  
        { 
         std::cout<<"Sent"<<*it<<_client_fd<<std::endl  ;  
            ssize_t bytes_sent = send(_client_fd, it->c_str(), it->size(), MSG_DONTWAIT);
            if (bytes_sent < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    break;
                } else {
                    break;
                }
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
