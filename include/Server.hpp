#include <sys/types.h>       
#include <sys/socket.h>      // socket(), bind(), listen(), accept(), etc.
#include <netinet/in.h>   
#include <arpa/inet.h>  
#include <unistd.h>      
#include <fcntl.h>          
#include <errno.h>      
#include <sys/epoll.h>      
#include <stdio.h>          
#include <string.h>          
#include "IEventHandler.hpp"  
#include <vector>

class  Server: public IEventHandler   
{       
     private :  
          int listen_fd ;     
          Server() ;  //----> i will listen here for new connections  // and we will  registre the listen_fd  to the Reactor matching with  Server Handler 
          Server(Server & copy  )  ;   
          virtual void handle_event()  ;      
     public  :    
          static Server&  getInstance() ;   
          ~Server() ;    
            Server&  operator=(Server &copy  ) ;  
          int getListenFd() ;    
            //implement  the Logic of accepting new connections// 
            
}   ;         