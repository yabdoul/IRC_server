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
    // Get the server parameter (optional)
    std::string server = "localhost";
    if (params.find("server") != params.end()) {
        server = params["server"];
    }
    
    // Send PONG response to the client
    // Format: :server PONG server :original_server_or_nick
    std::string pongMsg = ":localhost PONG localhost :" + server + "\r\n";
    
    sender.addMsg(pongMsg);
}
