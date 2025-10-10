#include "Reactor.hpp"  
#include "Server.hpp" 
#include "severResponsFactory.hpp"  
#include <string>  
#include "Client.hpp"  
#include "JoinCommand.hpp"  
#include "inviteCommand.hpp"  
#include "Parser.hpp"
#include <cstdlib>
#include <typeinfo>
#include <iostream>

int main(int argc, char **argv) 
{         
        if (argc != 3) {
                std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
                return 1;
        }

        int port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
                std::cerr << "Error: Invalid port number. Must be between 1-65535" << std::endl;
                return 1;
        }

        std::string password = argv[2];
        if (password.empty()) {
                std::cerr << "Error: Password cannot be empty" << std::endl;
                return 1;
        }

        try {
                std::cout << "Starting IRC server on port " << port << std::endl;
                Server::initServer(port, password);
                Reactor::getInstance().Run();   
        }  
        catch(const std::exception &e) {
                std::cerr << "Fatal error: " << e.what() << std::endl;
                std::cerr << "Exception type: " << typeid(e).name() << std::endl;
                return 1;
        }
        catch(...) {
                std::cerr << "Unknown fatal error occurred!" << std::endl;
                return 1;
        }

        return 0;
}

