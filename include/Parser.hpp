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
    Parser();
    Parser(const Parser&);
    Parser& operator=(const Parser&);
    void mapCommandParameters(const std::string& cmd, const std::vector<std::string>& parameters);
public:
    static Parser& getInstance();
    void parse(const std::string& rawCommand);
    const std::string& getPrefix() const;
    const std::string& getCommand() const;
    const std::map<std::string, std::string>& getParams() const;
};

#endif // PARSER_HPP
