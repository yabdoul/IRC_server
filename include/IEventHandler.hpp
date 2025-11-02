#ifndef IEventHandler_H 
#define IEventHandler_H

#include <sys/epoll.h>



class  IEventHandler 
{ 
     public :       
        IEventHandler(){} ;   
        ~IEventHandler() {} ;  
        IEventHandler(const IEventHandler &copy  )  { (void)copy; }   ;  
        IEventHandler& operator=(IEventHandler  & e )   ;   
       virtual void handle_event ( epoll_event e)  = 0 ;    
} ;    
#endif