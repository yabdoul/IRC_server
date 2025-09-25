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
#include <exception>    
#include <map>   

#define SERVER_NAME "irc_server_ysf"   

class ServerException : public std::exception
{
private:
     const std::string _reason;
public:   
    ServerException(const std::string &reason) : _reason("[ServerException]: " + reason)
    {
    }
    ~ServerException() throw() {}
    virtual char const *what() const throw()
    {
        return _reason.c_str();
    }
}  ;   
class  Server: public IEventHandler   
{       
     private :  
          int listen_fd ;       
          std::string  _serverName ;     
          int _port;
          std::string _password;
          Server() ;   
          Server(Server & copy  )  ;   
          virtual void handle_event(epoll_event   ev)  ;  
          std::map <std::string , Channel  >  ChannelList ;    
          std::vector<Client*>  _clientList  ;     
           bool _ready2Send       ;           
     public  :    
          static Server&  getInstance() ;
          void beReady2Send() { 
               _ready2Send = true  ;  
          } ;       
          bool isReady(){ return _ready2Send  ;   } ;   
          static void initServer(int port, const std::string& password);   
          ~Server() ;    
          Server&  operator=(Server &copy  ) ;  
          int getListenFd()  ;  
           Client&  getUser(std::string nickname )   ;    
          Channel  IsChannelExist(std::string &ChName   ) ;      
          Channel    AddChannel(std::string  &ChName )   ;  
          void UnsubscribeChannel (std::string &CName) ;     
          void saveUser(Client  *  c  ) ;  
          void callCommand(std::string& Command , std::map<std::string , std::string> & params  ,  Client &sender    )  ;       
          void Respond2User(int Client_fd ,  std::string resp )  ;
          const std::string& getPassword() const;      
     //--->ac         
}  ;          
