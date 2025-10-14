#include "PassCommand.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "severResponsFactory.hpp"

PassCommand::PassCommand() {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client &cl, std::map<std::string, std::string> &params) {
    // Check if password parameter exists
    if (!params.count("password") || params["password"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // Not enough parameters
        return;
    }

    // Check if client is already registered
    if (!cl.getNickName().empty() || !cl.getUsername().empty()) {
        cl.addMsg(serverResponseFactory::getResp(462, cl, params)); // Already registered
        return;
    }

    // Verify password against server password
    const std::string& serverPassword = Server::getInstance().getPassword();
    if (!serverPassword.empty() && params["password"] != serverPassword) {
        cl.addMsg(serverResponseFactory::getResp(464, cl, params)); // Password incorrect
        return;
    }

    // Password accepted - client is now authenticated
    cl.setAuthenticated(true);
}