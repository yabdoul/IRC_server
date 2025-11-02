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
#include <signal.h>


volatile sig_atomic_t shutdown_requested = 0;

void signal_handler(int signal) {
    (void)signal; 
    shutdown_requested = 1;
}

int main(int argc, char **argv) 
{         
        if (argc != 3) {
                return 1;
        }

        int port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
                return 1;
        }

        std::string password = argv[2];
        if (password.empty()) {
                return 1;
        }

        try {
                
                signal(SIGINT, signal_handler);   
                signal(SIGTERM, signal_handler);  
                
                Server::initServer(port, password);
                Reactor::getInstance().Run();   
                
                
                Server::getInstance().shutdown();
        }  
        catch(const std::exception &e) {
                return 1;
        }
        catch(...) {
                return 1;
        }

        return 0;
}

