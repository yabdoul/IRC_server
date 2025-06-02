#ifndef IEventHandler_H 
#define IEventHandler_H

#include <sys/epoll.h>

/**
 * @class IEventHandler
 * @brief Interface for handling events in an epoll-based event-driven system.
 * 
 * This class provides a pure virtual method `handle_event` that must be implemented
 * by derived classes to handle specific events. It also includes constructors and 
 * assignment operators for proper object management.
 * 
 * @note This is an abstract class and cannot be instantiated directly.
 * 
 * @method IEventHandler()
 * @brief Default constructor.
 * 
 * @method ~IEventHandler()
 * @brief Destructor.
 * 
 * @method IEventHandler(const IEventHandler &copy)
 * @brief Copy constructor.
 * @param copy The object to copy from.
 * 
 * @method IEventHandler& operator=(IEventHandler &e)
 * @brief Assignment operator.
 * @param e The object to assign from.
 * @return Reference to the assigned object.
 * 
 * @method virtual void handle_event(epoll_event e) = 0
 * @brief Pure virtual method to handle an epoll event.
 * @param e The epoll event to handle.
 */

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