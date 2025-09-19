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
        // std::vector<char> buffer(1024, '\0');
        std::string buffer ;   
        ssize_t n = recv(_client_fd, (void * )buffer.c_str(), buffer.size(), 0);
        if (n > 0) {
            // std::cout << std::string(buffer.begin(), buffer.begin() + n) << std::endl;        
            Parser::getInstance().parse(buffer) ;     
            Parser::getInstance().getCommand() ;   
            /*  
        [14-09 todo] --------> needed search  user with nickname in invite command  , ask the server for Client object using getClient functiion 
            */
            /*
                Call An Parser Instance Here , i think We need An Abstract factory of Move !  
                note: Parser function Should Return Command * !    
            */    
           
           

        }
    
    } else if (e.events & EPOLLOUT) {
        ;
        // std::cout << "Hello Output\n";
    }
}