#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>
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

// Client::Client() : _client_fd(-1), _Nick(""), _User("") {}

// Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO)
//     : _IO(IO), _Pass(Pass), _Nick(Nick), _User(User), _client_fd(client_fd) {}

Client::Client(int client_fd) : //   _IO(other._IO),
                                //   _Pass(other._Pass),
                                //   _Nick(other._Nick),
                                //   _User(other._User),
                                _client_fd(client_fd)
{
}

Client::~Client()
{
    close(_client_fd);
}

Client::Client(const Client &other) : IEventHandler(other),
                                      //   _Pass(other._Pass),
                                      //   _Nick(other._Nick),
                                      //   _User(other._User),
                                    _client_fd(other._client_fd)
{
}

void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0');
        ssize_t n = recv(_client_fd, buffer.data(), buffer.size(), 0);
        
        if (n > 0) {
            std::cout << std::string(buffer.begin(), buffer.begin() + n) << std::endl;
        }
    
    } else if (e.events & EPOLLOUT) {
        ;
        // std::cout << "Hello Output\n";
    }
}