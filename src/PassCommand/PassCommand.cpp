#include "PassCommand.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "severResponsFactory.hpp"

PassCommand::PassCommand() {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client &cl, std::map<std::string, std::string> &params) {  
    if (!params.count("password") || params["password"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params));
        return;
    }

    if (!cl.getNickName().empty() || !cl.getUsername().empty()) {
        cl.addMsg(serverResponseFactory::getResp(462, cl, params));
        return;
    }

    const std::string& serverPassword = Server::getInstance().getPassword();
    if (!serverPassword.empty() && params["password"] != serverPassword) {
        cl.addMsg(serverResponseFactory::getResp(464, cl, params));
        return;
    }
    cl.setAuthenticated(true);
}