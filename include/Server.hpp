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
#include "Channel.hpp" 

class  Server: public IEventHandler   
{       
     private :  
          int listen_fd ;     
          Server() ;  
          Server(Server & copy  )  ;   
          virtual void handle_event(epoll_event   ev)  ;  
          std::map <std::string , Channel  >  ChannelList ;       
     public  :    
          static Server&  getInstance() ;   
          ~Server() ;    
          Server&  operator=(Server &copy  ) ;  
          int getListenFd()  ;          
          Channel  IsChannelExist(Channel ch  ) ;   
          void AddChannel(std::string  ChName )   ;  
          void UnsubscribeChannel (const Channel& target)  ;         
}  ;          
