#include "Parser.hpp"
#include <sstream>
#include <algorithm>
#include <vector>
#include <cctype>

Parser::Parser() {}

Parser& Parser::getInstance() {
    static Parser instance;
    return instance;
}

void Parser::parse(const std::string& rawCommand) {
    prefix.clear();
    command.clear();
    params.clear();
    
    if (rawCommand.empty()) {
        return;
    }
    
    std::string cleanCommand = rawCommand;
    // Remove trailing \r\n if present
    if (cleanCommand.size() >= 2 && cleanCommand.substr(cleanCommand.size() - 2) == "\r\n") {
        cleanCommand = cleanCommand.substr(0, cleanCommand.size() - 2);
    }
    
    std::istringstream iss(cleanCommand);
    std::string token;
    std::vector<std::string> tokens;
    
    // Parse prefix if present
    if (!cleanCommand.empty() && cleanCommand[0] == ':') {
        iss >> token;
        prefix = token.substr(1); // Remove the ':' prefix
    }
    
    // Parse command
    if (iss >> token) {
        command = token;
        // Convert to uppercase for consistency
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    }
    
    // Parse remaining parameters
    std::vector<std::string> parameters;
    std::string remaining;
    
    // Check for trailing parameter (starts with ':')
    std::getline(iss, remaining);
    if (!remaining.empty() && remaining[0] == ' ') {
        remaining = remaining.substr(1); // Remove leading space
    }
    
    size_t colonPos = remaining.find(" :");
    std::string trailingParam;
    
    if (colonPos != std::string::npos) {
        trailingParam = remaining.substr(colonPos + 2); // +2 to skip " :"
        remaining = remaining.substr(0, colonPos);
    }
    
    // Parse space-separated parameters
    std::istringstream paramStream(remaining);
    std::string param;
    while (paramStream >> param) {
        parameters.push_back(param);
    }
    
    // Add trailing parameter if it exists
    if (!trailingParam.empty()) {
        parameters.push_back(trailingParam);
    }
    
    // Map parameters to appropriate keys based on command
    mapCommandParameters(command, parameters);
}

void Parser::mapCommandParameters(const std::string& cmd, const std::vector<std::string>& parameters) {
    if (cmd == "JOIN") {
        if (!parameters.empty()) {
            std::string channels = parameters[0];
            // Handle multiple channels separated by commas
            size_t pos = 0;
            std::string channel;
            while ((pos = channels.find(',')) != std::string::npos) {
                channel = channels.substr(0, pos);
                if (channel[0] == '#' || channel[0] == '&') {
                    params["channel"] = channel.substr(1); // Remove # or &
                    break; // Take first channel for now
                }
                channels.erase(0, pos + 1);
            }
            // Handle last/only channel
            if (!channels.empty() && (channels[0] == '#' || channels[0] == '&')) {
                params["channel"] = channels.substr(1);
            }
            // Handle password if provided
            if (parameters.size() > 1) {
                params["password"] = parameters[1];
            }
        }
    }
    else if (cmd == "PART") {
        if (!parameters.empty()) {
            std::string channel = parameters[0];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
            if (parameters.size() > 1) {
                params["message"] = parameters[1];
            }
        }
    }
    else if (cmd == "PRIVMSG" || cmd == "NOTICE") {
        if (parameters.size() >= 2) {
            std::string target = parameters[0];
            if (target[0] == '#' || target[0] == '&') {
                params["channel"] = target.substr(1);
                params["target_type"] = "channel";
            } else {
                params["nickname"] = target;
                params["target_type"] = "user";
            }
            params["message"] = parameters[1];
        }
    }
    else if (cmd == "KICK") {
        if (parameters.size() >= 2) {
            std::string channel = parameters[0];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
            params["nickname"] = parameters[1]; // User to kick
            if (parameters.size() > 2) {
                params["reason"] = parameters[2];
            }
        }
    }
    else if (cmd == "INVITE") {
        if (parameters.size() >= 2) {
            params["nickname"] = parameters[0]; // User to invite
            std::string channel = parameters[1];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
        }
    }
    else if (cmd == "MODE") {
        if (!parameters.empty()) {
            std::string target = parameters[0];
            if (target[0] == '#' || target[0] == '&') {
                params["channel"] = target.substr(1);
                params["target_type"] = "channel";
            } else {
                params["nickname"] = target;
                params["target_type"] = "user";
            }
            if (parameters.size() > 1) {
                params["mode"] = parameters[1];
            }
            if (parameters.size() > 2) {
                params["mode_param"] = parameters[2];
            }
        }
    }
    else if (cmd == "TOPIC") {
        if (!parameters.empty()) {
            std::string channel = parameters[0];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
            if (parameters.size() > 1) {
                params["topic"] = parameters[1];
            }
        }
    }
    else if (cmd == "NICK") {
        if (!parameters.empty()) {
            params["nickname"] = parameters[0];
        }
    }
    else if (cmd == "USER") {
        if (parameters.size() >= 4) {
            params["username"] = parameters[0];
            params["hostname"] = parameters[1];
            params["servername"] = parameters[2];
            params["realname"] = parameters[3];
        }
    }
    else if (cmd == "PASS") {
        if (!parameters.empty()) {
            params["password"] = parameters[0];
        }
    }
    else if (cmd == "QUIT") {
        if (!parameters.empty()) {
            params["message"] = parameters[0];
        }
    }
    else if (cmd == "WHO") {
        if (!parameters.empty()) {
            std::string target = parameters[0];
            if (target[0] == '#' || target[0] == '&') {
                params["channel"] = target.substr(1);
                params["target_type"] = "channel";
            } else {
                params["mask"] = target;
                params["target_type"] = "mask";
            }
        }
    }
    else if (cmd == "WHOIS") {
        if (!parameters.empty()) {
            params["nickname"] = parameters[0];
        }
    }
    else if (cmd == "LIST") {
        if (!parameters.empty()) {
            params["channels"] = parameters[0];
        }
    }
    else if (cmd == "NAMES") {
        if (!parameters.empty()) {
            std::string channel = parameters[0];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
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
