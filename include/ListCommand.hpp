#include "Command.hpp"
#include "severResponsFactory.hpp"

#pragma once

class ListCommand : public Command
{
    private:
        void execute(Client& sender, std::map<std::string, std::string>& params);
        
        // Helper methods for channel filtering
        bool matchesPattern(const std::string& channelName, const std::string& pattern);
        bool isChannelVisible(const Channel& channel, const Client& client);
        
    public:
        ListCommand();
        ~ListCommand();
};