#include "QuitCommand.hpp"
#include "Server.hpp"
#include <unistd.h>

QuitCommand::QuitCommand()
{
}

QuitCommand::~QuitCommand()
{
}

void QuitCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{
    std::string quitMessage = "Leaving";
    if (params.find("message") != params.end()) {
        quitMessage = params["message"];
    }
    
    std::string quitMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                         "@localhost QUIT :" + quitMessage + "\r\n";
    sender.addMsg(quitMsg);  
    
    
    
    close(sender.getClientFd());
    sender.markDisconnected();
}
