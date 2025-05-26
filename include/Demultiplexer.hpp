#include <sys/types.h>       // Basic system types
#include <sys/socket.h>      // socket(), bind(), listen(), accept(), etc.
#include <netinet/in.h>      // sockaddr_in, htons(), etc.
#include <arpa/inet.h>       // inet_addr(), inet_ntoa(), etc.
#include <unistd.h>          // close()
#include <fcntl.h>           // fcntl() for non-blocking mode
#include <errno.h>           // errno, EAGAIN, etc.
#include <sys/epoll.h>       // epoll_create(), epoll_ctl(), epoll_wait()
#include <stdio.h>           // printf(), perror()
#include <string.h>          // memset(), etc.

class Demultiplexer 
{    
    public  :  
        Demultiplexer()    ;    
        Demultiplexer(Demultiplexer &copy)  ;  
        Demultiplexer& operator=(Demultiplexer & copy )    ;     
        Demultiplexer(int socket_fd );  
        ~Demultiplexer(){} ;         

}   ;     