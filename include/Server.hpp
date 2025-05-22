#include "EventHandler.hpp" 

class  Server : public   EventHandler   
{       
       public  :   
            Server() ;  //----> i will listen here for new connections  // and we will  registre the listen_fd  to the Reactor matching with  Server Handler 
            Server(Server & copy  )  ;   
            ~Server() ;    
            Server&  operator=(Server &copy  ) ;    
            //implement  the Logic of accepting new connections// 
            virtual void handle_event() ;      
            
}   ;         