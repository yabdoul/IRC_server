#include "Parser.hpp"
#include <sstream>
#include <algorithm>

Parser::Parser() {}

Parser& Parser::getInstance() {
    static Parser instance;
    return instance;
}

void Parser::parse(const std::string& rawCommand) {
    prefix.clear();
    command.clear();
    params.clear();
    std::istringstream iss(rawCommand);
    std::string token;
    // Parse prefix if present
    if (!rawCommand.empty() && rawCommand[0] == ':') {
        iss >> token;
        prefix = token.substr(1);
    }
    // Parse command
    if (iss >> token) {
        command = token;
    }
    // Parse params as key=value pairs
    while (iss >> token) {
        size_t eq = token.find('=');
        if (eq != std::string::npos) {
            std::string key = token.substr(0, eq);
            std::string value = token.substr(eq + 1);
            params[key] = value;
        }
    }
}

const std::string& Parser::getPrefix() const {
    return prefix;
}

const std::string& Parser::getCommand() const {
    return command;
}

const std::map<std::string, std::string>& Parser::getParams() const {
    return params;
}
