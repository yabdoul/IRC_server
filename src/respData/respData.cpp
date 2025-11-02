#include "respData.hpp"   

respData::respData() {}
respData::~respData() {}

std::map<std::string , std::string> respData::getRespData(Client &cl  ,    std::map<std::string  , std::string > &params)
{  
    std::map<std::string , std::string> result;
    
    
    result["nick"] = cl.getNickname();
    result["user"] = cl.getUsername();
    result["realName"] = cl.getRealName();
    result["host"] = "localhost";  
    
    
    result["server"] = "irc.42server.com";
    result["version"] = "1.0.0";
    result["date"] = "2024-09-26";
    result["serverInfo"] = "42 IRC Server";
    
    
    result["userModes"] = "iow";
    result["channelModes"] = "itkol";
    
    
    result["users"] = "1";
    result["invisible"] = "0";
    result["servers"] = "1";
    result["ops"] = "1";
    result["unknown"] = "0";
    result["channels"] = "0";
    
    
    result["symbol"] = "=";  
    result["nickList"] = cl.getNickname();  
    result["userCount"] = "1";
    result["modes"] = "+nt";  
    result["topicTimestamp"] = "1640995200";  
    result["motdLine"] = "Welcome to 42 IRC Server!";
    result["awayMessage"] = "";
    
    
    result.insert(params.begin(), params.end());

    return result;
}