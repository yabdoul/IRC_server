#include "Client.hpp"  
#include <fcntl.h> 
#include <unistd.h> 

class ClientException : public std::exception {
     private:
     const std::string _reason;
     
     public:
     ClientException(const std::string &reason) : _reason("[ClientException]: " + reason) {  
          
     }
     virtual char const *what() const throw() {
          return _reason.c_str();
     }
};

Client::Client() : _client_fd(-1), _Nick(""), _User("") {}   

Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO):_client_fd(client_fd), _Nick(Nick), _User(User) , _Pass(Pass)  , _IO(IO){} 

Client::~Client()
{
     close(_client_fd) ;  
}  
Client::Client(const Client& other)
    : _client_fd(other._client_fd),
      _Nick(other._Nick),
      _User(other._User),
      _Pass(other._Pass),
      _IO(other._IO) 
{ } 