#ifndef USERCOMMAND_HPP
#define USERCOMMAND_HPP

#include "Command.hpp"
#include <map>
#include <string>

class UserCommand : public Command {
public:
    UserCommand();
    virtual ~UserCommand();
    virtual void execute(Client &cl, std::map<std::string, std::string> &params);
};

#endif