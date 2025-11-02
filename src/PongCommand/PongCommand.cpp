#include "PongCommand.hpp"
#include "Server.hpp"

PongCommand::PongCommand()
{
}

PongCommand::~PongCommand()
{
}

void PongCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{
    
    
    
    
    
    std::string server = "localhost";
    if (params.find("server") != params.end()) {
        server = params["server"];
    }
    
    
    
    
    
    
    
    
    
    
    
    
    (void)sender;  
    (void)server;  
}
