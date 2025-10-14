#include "nickCommand.hpp" 
#include "Server.hpp"
#include "severResponsFactory.hpp"
    

void nickCommand::execute(Client &cl,   std::map<std::string, std::string> &params)
{  
    if (!params.count("nickname") || params["nickname"].empty())
    {
        cl.addMsg(serverResponseFactory::getResp(431, cl, params)); 
        return;
    }

    std::string newNick = params["nickname"];

    if (cl.getNickName() == newNick)
    {
        return;
    }

    if (!Server::getInstance().isNickAvai(newNick))
    {
        cl.addMsg(serverResponseFactory::getResp(433, cl, params)); 
        return;
    }

    if (newNick.size() < 1 || newNick.size() > 9)
    {
        cl.addMsg(serverResponseFactory::getResp(432, cl, params)); 
        return;
    }
    if (newNick.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-[]\\`^{}_") != std::string::npos)
    {
        cl.addMsg(serverResponseFactory::getResp(432, cl, params));
        return;
    }

    if (!isalpha(newNick[0]) && std::string("-[]\\`^{}_").find(newNick[0]) == std::string::npos)
    {
        cl.addMsg(serverResponseFactory::getResp(432, cl, params));
        return;
    }

    cl.setNickName(newNick);   
std::string resp = ":" + std::string(SERVER_NAME) + " NICK " + newNick ;
 
    cl.addMsg( resp  )  ;   

}