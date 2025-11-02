#include <sys/types.h>       
#include <sys/socket.h>      
#include <netinet/in.h>      
#include <arpa/inet.h>       
#include <unistd.h>          
#include <fcntl.h>           
#include <errno.h>           
#include <sys/epoll.h>       
#include <stdio.h>           
#include <string.h>          

class Demultiplexer 
{    
    public  :  
        Demultiplexer()    ;    
        Demultiplexer(Demultiplexer &copy)  ;  
        Demultiplexer& operator=(Demultiplexer & copy )    ;     
        Demultiplexer(int socket_fd );  
        ~Demultiplexer(){} ;         

}   ;     