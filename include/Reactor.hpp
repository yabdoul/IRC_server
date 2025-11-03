#include <map>   
#include "IEventHandler.hpp" 
#include <iostream>         
#include <sys/epoll.h>
#include <exception>

class Reactor 
{  
    public :  
        static Reactor& getInstance()      ;  
        Reactor(Reactor &copy  )  ;  
        Reactor& operator=(Reactor &copy ) ;      
        void   Run()   ;  
        void    registre(epoll_event  ev ,  IEventHandler  * e  )    ;   
        void    unregistre(epoll_event ev)  ;
        int Dispatch()  ;     
        void notify(epoll_event  event ) ;        
        bool isUp  ;   
        int   getFd()   ;       
        void Shutdown() ;   
    private: 
        Reactor();  
        ~Reactor();
    private: 
        int _epoll_fd; 
        std::map<int , IEventHandler *   >  _registred   ;       
}   ;  
