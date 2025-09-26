#include "ListCommand.hpp"
#include "Server.hpp"
#include <algorithm>

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

bool ListCommand::isChannelVisible(const Channel& channel, const Client& client)
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
    // Send RPL_LISTSTART (321)
    sender.addMsg(serverResponseFactory::getResp(321, sender));
    
    // Get all channels from server
    const std::map<std::string, Channel>& channels = Server::getInstance().getChannelList();
    
    // Parse channel filters if provided
    std::vector<std::string> filters;
    if (params.find("channels") != params.end()) {
        std::string channelParam = params["channels"];
        
        // Split comma-separated channel list
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
    
    // Iterate through all channels
    for (std::map<std::string, Channel>::const_iterator it = channels.begin(); 
         it != channels.end(); ++it) {
        
        const std::string& channelName = it->first;
        const Channel& channel = it->second;
        
        // Check visibility
        if (!isChannelVisible(channel, sender)) {
            continue;
        }
        
        // Apply filters if specified
        bool matchesFilter = filters.empty();  // If no filters, show all
        
        for (size_t i = 0; i < filters.size(); ++i) {
            std::string filter = filters[i];
            
            // Remove # prefix from filter if present
            if (!filter.empty() && filter[0] == '#') {
                filter = filter.substr(1);
            }
            
            if (matchesPattern(channelName, filter)) {
                matchesFilter = true;
                break;
            }
        }
        
        if (!matchesFilter) {
            continue;
        }
        
        // Get channel information
        std::string topic = channel.getTopic();
        if (topic.empty()) {
            topic = "";
        }
        
        // Count users in channel (for now, we'll use a placeholder)
        // TODO: Implement actual user count from Channel class
        std::string userCount = "1";  // Placeholder
        
        // Send RPL_LIST (322) for this channel
        // Format: 322 nick #channel userCount :topic
        std::string listMsg = serverResponseFactory::getResp(322, sender);
        
        // Replace placeholders with actual values
        // This is a simplified approach - ideally serverResponseFactory would handle this
        sender.addMsg(listMsg);
    }
    
    // Send RPL_LISTEND (323)
    sender.addMsg(serverResponseFactory::getResp(323, sender));
}