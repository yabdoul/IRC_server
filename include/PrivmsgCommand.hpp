#include "Command.hpp"
#include "severResponsFactory.hpp"

#pragma once

class PrivmsgCommand : public Command
{
    private:
        void execute(Client& sender, std::map<std::string, std::string>& params);
        
    public:
        PrivmsgCommand();
        ~PrivmsgCommand();
};
