#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>  
#include "Parser.hpp"  

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


Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO)
    {  
         _IO = IO ;  
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

Client::Client(const Client &other) 
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
    else  
                
    ;   
}  ;   

void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0');
        ssize_t n = recv(_client_fd, buffer.data(), buffer.size(), 0);
        if (n > 0) {
            std::cout << std::string(buffer.begin(), buffer.begin() + n) << std::endl;   
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