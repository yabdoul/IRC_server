#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>  
#include "Parser.hpp"  
#include <cstdlib>  
#include <sstream> 
#include "Channel.hpp"

inline std::string SSTR(int x) {
    std::ostringstream oss;
    oss << std::dec << x;
    return oss.str();
}



class ClientException : public std::exception
{
private:
    const std::string _reason;

public:
    ClientException(const std::string &reason) : _reason("[ClientException]: " + reason)
    {
    }
    ~ClientException() throw() {}
    virtual char const *what() const throw()
    {
        return _reason.c_str();
    }
};  


Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass)
    {    
         _client_fd = client_fd ;  
         _Nick =  Nick ;  
         _User = User  ;  
         _Pass = Pass ;  
    }

Client::Client(int client_fd) :_client_fd(client_fd)
{}

Client::~Client()
{   if(_client_fd) close(_client_fd);
}

Client::Client(const Client &other):IEventHandler(other)
{  
     _Nick =  other._Nick ;   
     _Pass = other._Pass ;     
}  

void Client::rcvMsg(std::string &Msg  )  const  
{ 
    /*
    :  Send MSG via Socket to user fd ,  To complet   
    */  
    if(send(_client_fd ,  Msg.c_str()  , Msg.size() ,MSG_DONTWAIT ) < 0)  
        std::cout << "Problem in Sending Msg" << std::endl;   
                
    ;   
}  ;   

  std::map<std::string ,  std::string> Client::userData () const   
{
        std::map<std::string ,  std::string> result ;   
        result["_client_fd"] = SSTR(_client_fd).c_str()      ;    
        result["nickname"]   = _Nick ;    
        result["user"] =  _User ;    
        result["server"] = "ascasc" ;   
        return  result  ;   
}  ;     
 

void Client::subscribe2channel(Channel &ch )  
{   
    if(std::find(_subscribed2Channel.begin() ,  _subscribed2Channel.end()  ,  &ch) == _subscribed2Channel.end() )
      _subscribed2Channel.push_back(&ch) ;    
    else 
        throw std::runtime_error("[Already in The Channel]") ;   
} ;   


void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0')  ;  
        ssize_t n = recv(_client_fd, (void * )buffer.data(), buffer.size(), 0);
        if (n > 0) {  
            std::cout<<"RAW string"<<buffer.data()<<std::endl ;  
            // Parser::getInstance().parse(buffer.data()) ;             
            // std::map<std::string , std::string> parsingMap  =  Parser::getInstance().getParams()  ;    
            // for(std::map<std::string  , std::string>::iterator it  =   parsingMap.begin() ;   it !=  parsingMap.end() ;  it++   ) 
            // { 
            //      std::cout<<it->first<<"  "<<it->second<<std::endl ;      
            // } ; 
            // std::cout<<"i've recieved Something"<<std::endl ;   

        }
    
    } else if (e.events & EPOLLOUT) {
        ;
    }
}