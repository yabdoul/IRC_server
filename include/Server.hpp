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
          Server() ;  
          Server(Server & copy  )  ;   
          virtual void handle_event(epoll_event   ev)  ;      
     public  :    
          static Server&  getInstance() ;   
          ~Server() ;    
            Server&  operator=(Server &copy  ) ;  
          int getListenFd() ;              
}   ;         
