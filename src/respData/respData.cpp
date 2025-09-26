#include "respData.hpp"   

respData::respData() {}
respData::~respData() {}

std::map<std::string , std::string> respData::getRespData(Client &cl  ,    std::map<std::string  , std::string > &params)
{  
    std::map<std::string , std::string> result;
    
    // Basic client information
    result["nick"] = cl.getNickname();
    result["user"] = cl.getUsername();
    result["realName"] = cl.getRealName();
    result["host"] = "localhost";  // You may want to get actual hostname
    
    // Server information  
    result["server"] = "irc.42server.com";
    result["version"] = "1.0.0";
    result["date"] = "2024-09-26";
    result["serverInfo"] = "42 IRC Server";
    
    // Mode information
    result["userModes"] = "iow";
    result["channelModes"] = "itkol";
    
    // Server statistics (you may want to get these dynamically)
    result["users"] = "1";
    result["invisible"] = "0";
    result["servers"] = "1";
    result["ops"] = "1";
    result["unknown"] = "0";
    result["channels"] = "0";
    
    // Default values for common placeholders
    result["symbol"] = "=";  // For channel listings
    result["nickList"] = cl.getNickname();  // Will be overridden for actual channel lists
    result["userCount"] = "1";
    result["modes"] = "+nt";  // Default channel modes
    result["topicTimestamp"] = "1640995200";  // Unix timestamp
    result["motdLine"] = "Welcome to 42 IRC Server!";
    result["awayMessage"] = "";
    
    // Insert any additional parameters passed in
    result.insert(params.begin(), params.end());

    return result;
}