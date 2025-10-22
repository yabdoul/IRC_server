#include "Parser.hpp"
#include <sstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <iostream>

Parser::Parser() : isValid(false) {}

Parser& Parser::getInstance() {
    static Parser instance;
    return instance;
}

bool Parser::parse(const std::string& rawCommand) {
    prefix.clear();
    command.clear();
    params.clear();
    lastError.clear();
    isValid = false;
    
    if (rawCommand.empty()) {
        lastError = "Empty command";
        return false;
    }
    
    std::string cleanCommand = rawCommand;
    if (!sanitizeInput(cleanCommand)) {
        return false;
    }
    
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
    
    // Validate command
    if (!validateCommand(command)) {
        return false;
    }
    
    // Map parameters to appropriate keys based on command
    mapCommandParameters(command, parameters);
    
    isValid = true;
    return true;
}

// Input sanitization to prevent injection attacks
bool Parser::sanitizeInput(std::string& input) {
    // Remove null bytes and dangerous control characters
    for (size_t i = 0; i < input.length(); ++i) {
        unsigned char c = input[i];
        if (c == 0 || (c < 32 && c != '\r' && c != '\n')) {
            input.erase(i, 1);
            --i;
        }
    }
    
    // RFC 2812: Max message length is 512 characters including CRLF
    if (input.length() > 510) {
        input = input.substr(0, 510);
        lastError = "Message truncated: exceeds maximum length";
        return false;
    }
    
    return true;
}

// Validate IRC command names
bool Parser::validateCommand(const std::string& cmd) {
    if (cmd.empty()) {
        lastError = "Empty command";
        return false;
    }
    
    // Check if it's a numeric command (001, 002, etc.)
    bool isNumeric = true;
    for (size_t i = 0; i < cmd.length(); ++i) {
        if (!std::isdigit(cmd[i])) {
            isNumeric = false;
            break;
        }
    }
    
    if (isNumeric && cmd.length() == 3) {
        return true; // Valid numeric command
    }
    
    // List of valid IRC commands
    if (cmd == "PASS" || cmd == "NICK" || cmd == "USER" || cmd == "JOIN" ||
        cmd == "PART" || cmd == "PRIVMSG" || cmd == "NOTICE" || cmd == "KICK" ||
        cmd == "INVITE" || cmd == "MODE" || cmd == "TOPIC" || cmd == "QUIT" ||
        cmd == "WHO" || cmd == "WHOIS" || cmd == "LIST" || cmd == "NAMES" ||
        cmd == "PING" || cmd == "PONG") {
        return true;
    }
    
    lastError = "Unknown command: " + cmd;
    return false;
}

// Validate IRC nickname format
bool Parser::validateNickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9) {
        return false;
    }
    
    // First character must be letter or special char
    char first = nick[0];
    if (!std::isalpha(first) && first != '[' && first != ']' && 
        first != '\\' && first != '`' && first != '_' && 
        first != '^' && first != '{' && first != '|' && first != '}') {
        return false;
    }
    
    // Remaining chars: letters, digits, or special chars
    for (size_t i = 1; i < nick.length(); ++i) {
        char c = nick[i];
        if (!std::isalnum(c) && c != '[' && c != ']' && c != '\\' && 
            c != '`' && c != '_' && c != '^' && c != '{' && c != '|' && 
            c != '}' && c != '-') {
            return false;
        }
    }
    return true;
}

// Validate IRC channel name format
bool Parser::validateChannelName(const std::string& channel) {
    if (channel.empty() || channel.length() > 50) {
        return false;
    }
    
    if (channel[0] != '#' && channel[0] != '&') {
        return false;
    }
    
    // No spaces, commas, or control characters
    for (size_t i = 1; i < channel.length(); ++i) {
        char c = channel[i];
        if (c == ' ' || c == ',' || c == '\0' || c == '\r' || c == '\n' || c == 7) {
            return false;
        }
    }
    return true;
}

// Handle multiple commands in one buffer
std::vector<std::string> Parser::parseMultiple(const std::string& buffer) {
    std::vector<std::string> commands;
    std::string tempBuffer = buffer;
    
    size_t pos = 0;
    while ((pos = tempBuffer.find("\r\n")) != std::string::npos) {
        std::string command = tempBuffer.substr(0, pos);
        if (!command.empty()) {
            commands.push_back(command);
        }
        tempBuffer.erase(0, pos + 2);
    }
    
    return commands;
}

// Handle partial messages in buffer
std::string Parser::parsePartialBuffer(std::string& buffer) {
    size_t pos = 0;
    
    while ((pos = buffer.find("\r\n")) != std::string::npos) {
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 2);
        
        if (!command.empty()) {
            if (!parse(command)) {
                // Log parsing error but continue
                std::cerr << "Parse error: " << lastError << std::endl;
            }
        }
    }
    
    // Return remaining partial message
    return buffer;
}

void Parser::mapCommandParameters(const std::string& cmd, const std::vector<std::string>& parameters) {
    if (cmd == "JOIN") {
        if (!parameters.empty()) {
            std::string channels = parameters[0];
            size_t pos = 0;
            std::string channel;
            while ((pos = channels.find(',')) != std::string::npos) {
                channel = channels.substr(0, pos);
                if (channel[0] == '#' || channel[0] == '&') {
                    params["channel"] = channel.substr(1); 
                    break;
                }
                channels.erase(0, pos + 1);
            }
            if (!channels.empty() && (channels[0] == '#' || channels[0] == '&')) {
                params["channel"] = channels.substr(1);
            }
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
            // strip leading ':' if present (robustness for malformed input)
            if (!target.empty() && target[0] == ':') {
                target = target.substr(1);
            }

            if (!target.empty()) {
                if (target[0] == '#' || target[0] == '&') {
                    params["channel"] = target.substr(1);
                    params["target_type"] = "channel";
                } else {
                    params["nickname"] = target;
                    params["target_type"] = "user";
                    command = "USERPRIV";
                }
            } else {
                // fallback: no explicit target provided, try to use prefix as nickname
                if (!prefix.empty()) {
                    params["nickname"] = prefix;
                    params["target_type"] = "user";
                    command = "USERPRIV";
                } else {
                    lastError = "Missing target for PRIVMSG/NOTICE";
                }
            }

            std::string msg = parameters[1];
            if (!msg.empty() && msg[0] == ':') {
                msg = msg.substr(1);
            }
            params["message"] = msg;
        }
        else if (parameters.size() == 1) {
            // Only message provided — treat as user-directed if we have a prefix
            std::string msg = parameters[0];
            if (!msg.empty() && msg[0] == ':') {
                msg = msg.substr(1);
            }
            params["message"] = msg;

            if (!prefix.empty()) {
                params["nickname"] = prefix;
                params["target_type"] = "user";
                command = "USERPRIV";
            } else {
                lastError = "Missing target for PRIVMSG/NOTICE";
            }
        }
    }
    else if (cmd == "KICK") {
        if (parameters.size() >= 2) {
            std::string channel = parameters[0];
            if (channel[0] == '#' || channel[0] == '&') {
                params["channel"] = channel.substr(1);
            }
            params["target"] = parameters[1]; // Users to kick (comma-separated)
            if (parameters.size() > 2) {
                params["reason"] = parameters[2];
            }
        }
    }
    else if (cmd == "INVITE") {
        if (parameters.size() >= 2) {
            params["nickname"] = parameters[0]; 
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
    else if (cmd == "PING") {
        if (!parameters.empty()) {
            params["server"] = parameters[0];
        }
    }
    else if (cmd == "PONG") {
        if (!parameters.empty()) {
            params["server"] = parameters[0];
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

 std::string& Parser::getPrefix()    {
    return  prefix;
}

 std::string&  Parser::getCommand()     {
    return command;
}

 std::map<std::string, std::string>&  Parser::getParams()  {
    return  params;
}



bool Parser::isValidParse()  const  {
    return isValid;
}

// Static validation helpers
bool Parser::isValidIRCMessage(const std::string& message) {
    if (message.empty() || message.length() > 512) {
        return false;
    }
    
    // Check for proper CRLF ending
    if (message.length() >= 2) {
        std::string ending = message.substr(message.length() - 2);
        if (ending != "\r\n") {
            return false;
        }
    }
    
    return true;
}

std::string Parser::sanitizeMessage(const std::string& message) {
    std::string result = message;
    
    // Remove dangerous characters
    for (size_t i = 0; i < result.length(); ++i) {
        unsigned char c = result[i];
        if (c == 0 || (c < 32 && c != '\r' && c != '\n')) {
            result.erase(i, 1);
            --i;
        }
    }
    
    return result;
}
