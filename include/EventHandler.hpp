class  EventHandler 
{ 
     public :       
        EventHandler() ;   
        ~EventHandler() {} ;  
        EventHandler(EventHandler &copy  )    ;  
        EventHandler& operator=(EventHandler  & e )   ;   
       virtual void handle_event ( )  =0 ;    
} ;    