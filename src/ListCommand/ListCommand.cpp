#include "ListCommand.hpp"
#include "Server.hpp"
#include <algorithm>
#include <iostream>
ListCommand::ListCommand()
{
}

ListCommand::~ListCommand()
{
}

bool ListCommand::matchesPattern(const std::string& channelName, const std::string& pattern)
{
    // Simple pattern matching with wildcards * and ?
    // * matches any sequence of characters
    // ? matches any single character
    
    size_t i = 0, j = 0;
    size_t starIdx = std::string::npos, match = 0;
    
    while (i < channelName.length()) {
        if (j < pattern.length() && (pattern[j] == '?' || pattern[j] == channelName[i])) {
            i++;
            j++;
        } else if (j < pattern.length() && pattern[j] == '*') {
            starIdx = j;
            match = i;
            j++;
        } else if (starIdx != std::string::npos) {
            j = starIdx + 1;
            match++;
            i = match;
        } else {
            return false;
        }
    }
    
    while (j < pattern.length() && pattern[j] == '*') {
        j++;
    }
    
    return j == pattern.length();
}

bool ListCommand::isChannelVisible( Channel& channel,  Client& client)
{
    // Check if channel is visible to the client
    // For now, we'll implement basic visibility:
    // - All channels are visible unless they have special modes
    // - In a full implementation, this would check for +s (secret) and +p (private) modes
    
    (void)channel;  // Suppress unused parameter warning
    (void)client;   // Suppress unused parameter warning
    
    // For basic implementation, all channels are visible
    // TODO: Implement channel mode checking (+s secret, +p private)
    return true;
}

void ListCommand::execute(Client& sender, std::map<std::string, std::string>& params)
{  
    sender.addMsg(serverResponseFactory::getResp(321, sender, params));
    
        std::vector<Channel *> channelList =  Server::getInstance().getChannelList() ;   
        std::vector<std::string> filters;
    if (params.find("channels") != params.end()) {
        std::string channelParam = params["channels"];
        
        std::string currentFilter = "";
        for (size_t i = 0; i < channelParam.length(); ++i) {
            if (channelParam[i] == ',') {
                if (!currentFilter.empty()) {
                    filters.push_back(currentFilter);
                    currentFilter = "";
                }
            } else {
                currentFilter += channelParam[i];
            }
        }
        if (!currentFilter.empty()) {
            filters.push_back(currentFilter);
        }
    }
    
    for (std::vector<Channel  *>::iterator it = channelList.begin();     
    it != channelList.end(); ++it) {
        std::cout<<"channel ,  loop"<<std::endl ;   
        
        if (!isChannelVisible( **it , sender)) {
            continue;
        }
        
        bool matchesFilter = filters.empty();  
        
        for (size_t i = 0; i < filters.size(); ++i) {
            std::string filter = filters[i];
            
            if (!filter.empty() && filter[0] == '#') {
                filter = filter.substr(1);
            }
            
            if (matchesPattern((*it)->getName(), filter)) {
                matchesFilter = true;
                break;
            }
        }
        
        if (!matchesFilter) {
            continue;
        }
        
        std::string topic = (*it)->getTopic();
        if (topic.empty()) {
            topic = "";
        }
        
        std::string userCount = "1";  // Placeholder
        std::string listMsg = serverResponseFactory::getResp(322, sender, params);
        sender.addMsg(listMsg);
    }
    sender.addMsg(serverResponseFactory::getResp(323, sender, params));
}