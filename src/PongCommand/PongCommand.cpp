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
    // PONG is typically a response to PING from server
    // When client sends PONG, it's acknowledging a server PING
    // This serves as a keepalive confirmation
    
    // Get the server parameter (what they're PONGing to)
    std::string server = "localhost";
    if (params.find("server") != params.end()) {
        server = params["server"];
    }
    
    // For now, we'll just acknowledge the PONG silently
    // In a more complete implementation, this could:
    // - Reset client timeout timers
    // - Update last activity timestamp
    // - Handle lag measurement
    
    // Optional: Could send confirmation back to client
    // std::string confirmMsg = ":localhost NOTICE " + sender.getNickname() + 
    //                         " :PONG acknowledged\r\n";
    // sender.addMsg(confirmMsg);
    
    (void)sender;  // Suppress unused parameter warning for now
    (void)server;  // Suppress unused parameter warning for now
}
