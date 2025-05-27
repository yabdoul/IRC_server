#include "IEventHandler.hpp"
#include <iostream>
#include "IOstream.hpp"

class Client {
private:
    int   _client_fd;
    IOstream* _IO;
    std::string _Nick;
    std::string _Pass;
    std::string _User;

public:

    ~Client();       
    Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass, IOstream* IO);
    Client() ;  

    Client(const Client& other);

    Client& operator=(const Client& other);

    Client(Client&& other) noexcept;

    Client& operator=(Client&& other) noexcept;

    virtual void handle_event();
};