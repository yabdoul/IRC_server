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
                    // Clean up disconnected clients after processing events
                    Server::getInstance().cleanupDisconnectedClients();
               } catch (std::exception &e) {
                    std::cerr << e.what() << "\n";
                    break;
               }
          }
          // terminate();
}  


/* Subscribing ev to the Reactor */
/**
 * @brief Registers an event handler with the reactor.
 *
 * This method adds a file descriptor to the epoll instance and associates it
 * with an event handler. If the file descriptor is already registered, an exception
 * is thrown.
 *
 * @param ev The epoll_event structure containing the file descriptor and event details.
 * @param e A pointer to the IEventHandler instance to handle events for the file descriptor.
 *
 * @throws ReactorException If epoll_ctl fails or if the file descriptor is already registered.
 */
void Reactor::registre(epoll_event ev, IEventHandler* e) {
    int fd = ev.data.fd;
   std::map<int , IEventHandler * >::iterator it  = _registred.find(fd);
    if (it == _registred.end()) {
        // fd not registered yet, add it
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {  
            throw ReactorException("Fatal: epoll_ctl() ADD failed");
        }
        _registred.insert(std::make_pair(fd, e));
    } else {
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            throw ReactorException("Fatal: epoll_ctl() MOD failed");
        }
        it->second = e;
    }
}

/**
 * @brief Unregisters a file descriptor from the reactor.
 *
 * This function removes the specified file descriptor from the epoll instance
 * and deletes its associated event handler from the registry. If the file
 * descriptor does not exist in the registry, a ReactorException is thrown.
 *
 * @param ev The epoll_event structure containing the file descriptor to be unregistered.
 * 
 * @throws ReactorException If the file descriptor does not exist in the registry.
 */
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

/**
 * @brief Notifies the appropriate event handler for a given epoll event.
 *
 * This function retrieves the event handler associated with the file descriptor
 * in the provided epoll event and invokes its `handle_event` method. If the file
 * descriptor is not registered, an exception is thrown.
 *
 * @param ev The epoll event containing the file descriptor and event data.
 * @throws ReactorException If the file descriptor in the event is not registered.
 */
void Reactor::notify(epoll_event   ev   )  
{  
     std::map<int  , IEventHandler *>::iterator it = _registred.find(ev.data.fd  ) ;    
     if (it  == _registred.end())
          throw ReactorException("Fd not Registred") ;    
     (*it).second->handle_event(ev) ; 
}