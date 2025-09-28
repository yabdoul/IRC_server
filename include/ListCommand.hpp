#include "Command.hpp"
#include "severResponsFactory.hpp"

#pragma once

class ListCommand : public Command
{
    private:
    
    bool matchesPattern(const std::string& channelName, const std::string& pattern);
    bool isChannelVisible( Channel& channel,  Client& client);
    public:
        void execute(Client& sender, std::map<std::string, std::string>& params);
        ListCommand();
        ~ListCommand();
};