#include "UserCommand.hpp"
#include "Client.hpp"
#include "severResponsFactory.hpp"
#include "Server.hpp"

UserCommand::UserCommand() {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client &cl, std::map<std::string, std::string> &params) {
    if (!params.count("username") || params["username"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }

    if (!params.count("hostname") || params["hostname"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }

    if (!params.count("servername") || params["servername"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }

    if (!params.count("realname") || params["realname"].empty()) {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params)); 
        return;
    }
    cl.setUser(params["username"]);

    const std::string &serverPass = Server::getInstance().getPassword();
    if (!serverPass.empty() && cl.getState() != Client::PASSWORD_SET) {
        cl.addMsg(serverResponseFactory::getResp(464, cl, params));
        return;
    }

    if (!cl.getNickName().empty() && !cl.getUsername().empty()) {
        cl.addMsg(serverResponseFactory::getResp(001, cl, params));
        cl.addMsg(serverResponseFactory::getResp(002, cl, params));
        cl.addMsg(serverResponseFactory::getResp(003, cl, params));
        cl.addMsg(serverResponseFactory::getResp(004, cl, params));
        
        cl.setState(Client::REGISTERED);
    }
}