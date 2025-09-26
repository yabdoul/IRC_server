#include "QuitCommand.hpp"
#include "Server.hpp"

QuitCommand::QuitCommand()
{
}

QuitCommand::~QuitCommand()
{
}

void QuitCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{
    // Get quit message (optional)
    std::string quitMessage = "Leaving";
    if (params.find("message") != params.end()) {
        quitMessage = params["message"];
    }
    
    // Create QUIT message to broadcast: :nick!user@host QUIT :message
    std::string quitMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                         "@localhost QUIT :" + quitMessage + "\r\n";
    
    // Broadcast quit message to all channels the user is in
    // We need to access the user's subscribed channels
    // Since we can't directly access _subscribed2Channel, we'll need to work with
    // the server to handle channel cleanup
    
    // Send quit message to the user themselves
    sender.addMsg(quitMsg);
    
    // Note: The actual channel cleanup and disconnection should be handled 
    // by the server after this command completes. The QUIT command indicates
    // the user's intention to disconnect, but the server manages the cleanup.
}
