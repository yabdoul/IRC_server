#ifndef IEventHandler_H 
#define IEventHandler_H
class  IEventHandler 
{ 
     public :       
        IEventHandler(){} ;   
        ~IEventHandler() {} ;  
        IEventHandler(IEventHandler &copy  )    ;  
        IEventHandler& operator=(IEventHandler  & e )   ;   
       virtual void handle_event ( )  = 0 ;    
} ;    
#endif