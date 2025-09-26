#include "Command.hpp"
#include "severResponsFactory.hpp"

#pragma once

class PongCommand : public Command
{
    private:
        void execute(Client& sender, std::map<std::string, std::string>& params);
        
    public:
        PongCommand();
        ~PongCommand();
};
