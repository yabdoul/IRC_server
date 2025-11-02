#include "PingCommand.hpp"
#include "Server.hpp"

PingCommand::PingCommand()
{
}

PingCommand::~PingCommand()
{
}

void PingCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{
    
    std::string server = "localhost";
    if (params.find("server") != params.end()) {
        server = params["server"];
    }
    
    
    
    std::string pongMsg = ":localhost PONG localhost :" + server + "\r\n";
    
    sender.addMsg(pongMsg);
}
