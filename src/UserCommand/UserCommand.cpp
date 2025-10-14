#include "UserCommand.hpp"
#include "Client.hpp"
#include "severResponsFactory.hpp"

UserCommand::UserCommand() {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client &cl, std::map<std::string, std::string> &params) {
    // Check if we have the required parameters
    if (!params.count("username") || params["username"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // Not enough parameters
        return;
    }

    if (!params.count("hostname") || params["hostname"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // Not enough parameters
        return;
    }

    if (!params.count("servername") || params["servername"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // Not enough parameters
        return;
    }

    if (!params.count("realname") || params["realname"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); // Not enough parameters
        return;
    }

    // Set user information
    cl.setUser(params["username"]);
    
    // Check if client is fully registered (has both nick and user)
    if (!cl.getNickName().empty() && !cl.getUsername().empty()) {
        // Send welcome messages (001-004)
        cl.addMsg(serverResponseFactory::getResp(001, cl, params));
        cl.addMsg(serverResponseFactory::getResp(002, cl, params));
        cl.addMsg(serverResponseFactory::getResp(003, cl, params));
        cl.addMsg(serverResponseFactory::getResp(004, cl, params));
    }
}