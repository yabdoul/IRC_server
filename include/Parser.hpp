#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <map>
#include <vector>

class Parser {
private:
    std::string prefix;
    std::string command;
    std::map<std::string, std::string> params;
    std::string lastError;
    bool isValid;
    
    Parser();
    Parser(const Parser&);
    Parser& operator=(const Parser&);
    void mapCommandParameters(const std::string& cmd, const std::vector<std::string>& parameters);
    bool validateCommand(const std::string& cmd);
    bool sanitizeInput(std::string& input);
    bool validateNickname(const std::string& nick);
    bool validateChannelName(const std::string& channel);
    
public:
    static Parser& getInstance();
    bool parse(const std::string& rawCommand);
    std::vector<std::string> parseMultiple(const std::string& buffer);
    std::string parsePartialBuffer(std::string& buffer);
    
     std::string& getPrefix() ;
     std::string&  getCommand() ;
     std::map<std::string, std::string>& getParams() ;
     std::string& getLastError() const;
    bool isValidParse() const;
    
    
    static bool isValidIRCMessage(const std::string& message);
    static std::string sanitizeMessage(const std::string& message);
};

#endif 
