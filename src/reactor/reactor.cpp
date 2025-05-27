#include "Reactor.hpp"  
#include "Server.hpp"  
#include "Demultiplexer.hpp"
#include <algorithm>


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

} 

Reactor::~Reactor() {
     close(_epoll_fd);
}   

 Reactor&  Reactor::getInstance()   
{   
     static Reactor Reactor ;  
     return Reactor ;     
}   
        

void    Reactor::Run() 
{     
          Server::getInstance() ;   
          while(1)
          {
               try {
                    Demultiplexer();    
               } catch (std::exception &e) {
                    std::cerr << e.what() << "\n";
                    break;
               }
          }
          // terminate();
}  


/* Subscribing ev to the Reactor */
void   Reactor::registre(epoll_event ev , IEventHandler * e  )  {   


     if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev)  == -1)  
          throw ReactorException("Fatal: epoll_ctl() faild");
     std::map<int, IEventHandler *>::iterator it = _registred.find(ev.data.fd);
     if (it != _registred.end())
          {   
                throw ReactorException("Fd Already exist");  
          }  
     int fd = ev.data.fd;
     _registred.insert(std::make_pair(fd , e));
} 

void   Reactor::unregistre(epoll_event ev) 
{   
     std::map<int  , IEventHandler *>::iterator it = _registred.find(ev.data.fd  ) ;    
     if (it  == _registred.end())
          throw ReactorException("fd dont exists")  ;   
     epoll_ctl(_epoll_fd ,  EPOLL_CTL_DEL , ev.data.fd , &ev )  ;    
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