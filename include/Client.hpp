#include "IEventHandler.hpp"
#include <iostream>
#include "IOstream.hpp"
#include <sys/socket.h>
#include <vector>

class Client : public IEventHandler {
private:
    IOstream* _IO;
    int   _client_fd;
    // std::string _Nick;
    // std::string _Pass;
    // std::string _User;

public:

    ~Client();       
    // Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO);
    Client(int client_fd);
    // Client() ;  

    Client& operator=(const Client& other);

    Client(const Client &other)  ;

    virtual void handle_event(epoll_event   ev); /* --> INPUT = parse request, (Eg.: JOIN)*/
};