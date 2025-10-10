#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "Command.hpp"
#include <map>
#include <string>

class PassCommand : public Command {
public:
    PassCommand();
    virtual ~PassCommand();
    virtual void execute(Client &cl, std::map<std::string, std::string> &params);
};

#endif