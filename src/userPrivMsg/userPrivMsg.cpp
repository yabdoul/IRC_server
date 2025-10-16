#include "userPrivMsg.hpp"   
#include "severResponsFactory.hpp" 
#include "Server.hpp"  

void userPrivMsg::execute(Client& sender, std::map<std::string, std::string>& params)
{
    std::cout << "Entered the user-to-user privmsg function" << std::endl;

    // Check required fields
    if (params.find("message") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(412, sender, params));
        return;
    }

    if (params.find("nickname") == params.end()) {
        sender.addMsg(serverResponseFactory::getResp(461, sender, params));  
        return;
    }

    std::string message = params["message"];
    std::string targetNick = params["nickname"];

    try {
        std::cout << "Target nick is " << targetNick << std::endl;
        Client& targetUser = Server::getInstance().getUser(targetNick);

        std::string privmsgMsg = ":" + sender.getNickname() + "!" + sender.getUsername() + 
                                 "@localhost PRIVMSG " + targetNick + " :" + message + "\r\n";

        targetUser.addMsg(privmsgMsg);
    } catch (std::exception& e) {
        sender.addMsg(serverResponseFactory::getResp(401, sender, params));
    }
}
