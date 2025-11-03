#include "Reactor.hpp"  
#include "Server.hpp"  
#include "Demultiplexer.hpp"
#include <algorithm>
#include <typeinfo>
#include <iostream>


class ReactorException : public std::exception {
     private:
     const std::string _reason;
     
     public:
     ReactorException(const std::string &reason) : _reason("[ReactorException]: " + reason) {  
     }
     ~ReactorException() throw() {

     } 
     virtual char const *what() const throw() {
          return _reason.c_str();
     }
};


Reactor::Reactor() : _epoll_fd(epoll_create(MAX_HANDLE_SZ)) {
     isUp =  false  ;   
} 

Reactor::~Reactor() {
     close(_epoll_fd);
}   

 Reactor&  Reactor::getInstance()   
{   
     static Reactor Reactor ;  
     return Reactor ;     
}   
        
void Reactor::Shutdown() 
{ 
      Server::getInstance().terminate() ;   
      
}
void    Reactor::Run() 
{         isUp = true ;   
          Server::getInstance() ;   
          while(isUp)
          {
               try {
                    Demultiplexer() ;
                    Server::getInstance().cleanupDisconnectedClients();
               } catch (const std::exception &e) {
                    break;
               } catch (...) {
                    break;
               }
          }
          Reactor::getInstance().Shutdown() ;    
}  




void Reactor::registre(epoll_event ev, IEventHandler* e) {
    int fd = ev.data.fd;
   std::map<int , IEventHandler * >::iterator it  = _registred.find(fd);
    if (it == _registred.end()) {
        
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {  
            throw ReactorException("Fatal: epoll_ctl() ADD failed");
        }
        _registred.insert(std::make_pair(fd, e));
    } else {
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            
            epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
            _registred.erase(it);
            if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
                
                return;
            }
            _registred.insert(std::make_pair(fd, e));
        } else {
            it->second = e;
        }
    }
}


void   Reactor::unregistre(epoll_event ev) 
{   
     std::map<int  , IEventHandler *>::iterator it = _registred.find(ev.data.fd  ) ;    
     if (it  == _registred.end()) {
          return;  
     }
     if (epoll_ctl(_epoll_fd ,  EPOLL_CTL_DEL , ev.data.fd , &ev ) == -1) {
          
     }
     _registred.erase(ev.data.fd ) ;  
} 


int  Reactor::getFd()
{    
       return _epoll_fd ;    
}  


void Reactor::notify(epoll_event   ev   )  
{  
     std::map<int  , IEventHandler *>::iterator it = _registred.find(ev.data.fd  ) ;    
     if (it  == _registred.end())
          throw ReactorException("Fd not Registred") ;    
     (*it).second->handle_event(ev) ; 
}